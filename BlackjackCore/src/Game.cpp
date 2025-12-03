#include "Game.h"
#include "Card.h"
#include "StandardDealerStrategy.h"
#include <algorithm>

Game::Game(int initialBalance)
    : deckPtr(&realDeck), player(initialBalance), dealerStrategy(std::make_unique<StandardDealerStrategy>())
{
    realDeck.shuffle();
}

void Game::forceDeck(IDeck* fake)
{
    this->deckPtr = fake;
}

void Game::setDealerStrategy(std::unique_ptr<IDealerStrategy> strategy)
{
    if (strategy) {
        dealerStrategy = std::move(strategy);
    }
}

void Game::notify(const std::string& event)
{
    for (auto* obs : observers)
        if (obs) obs->onNotify(event);
}

void Game::notifyObservers(const std::string& eventName)
{
    notify(eventName);
}

void Game::addObserver(IObserver* observer)
{
    if (observer) observers.push_back(observer);
}

void Game::removeObserver(IObserver* observer)
{
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void Game::reset()
{
    realDeck = Deck();
    realDeck.shuffle();
    deckPtr = &realDeck;

    player.resetForNewRound();
    dealerHand.clear();

    activeHand = ActiveHand::NONE;
    roundStarted = false;
    gameOver = false;

    splitAllowed = false;
    insuranceAllowed = false;
    doubleAllowed = false;

    dealerHasHiddenCard = false;
    dealerBlackjack = false;

    mainResult = EGameResult::None;
    splitResult = EGameResult::None;

    notify("Reset");
}

bool Game::requestBet(int amount)
{
    if (roundStarted || gameOver) return false;

    bool ok = player.bet(amount);
    if (ok) notify("BetChanged");
    return ok;
}

bool Game::requestClearBet()
{
    if (roundStarted || gameOver) return false;

    player.clearBet();
    notify("BetChanged");
    return true;
}

void Game::startGame()
{
    if (roundStarted || gameOver) return;
    if (player.getCurrentBet() <= 0) return;

    roundStarted = true;
    gameOver = false;
    activeHand = ActiveHand::MAIN;

    splitAllowed = false;
    insuranceAllowed = false;
    doubleAllowed = true;

    dealerHasHiddenCard = false;
    dealerBlackjack = false;

    dealInitialCards();

    dealerHasHiddenCard = (dealerHand.getCardCount() >= 2);

    if (dealerHand.getCardCount() > 0)
    {
        const ICard* visible = dealerHand.getCard(0);
        insuranceAllowed = (visible && visible->getValue() == ECardValue::Ace);
    }
    else
    {
        insuranceAllowed = false;
    }

    splitAllowed = player.canSplit();
    doubleAllowed = true;

    dealerBlackjack = isBlackjack(dealerHand);

    notify("StartGame");
}

void Game::hit()
{
    if (!roundStarted || gameOver) return;

    if (!player.hasSplitHand())
    {
        if (dealerBlackjack || isBlackjack(player.getMainHand()))
        {
            activeHand = ActiveHand::DONE;
            dealerHasHiddenCard = false;
            dealerPlay();
            evaluateHands();
            gameOver = true;
            notify("RoundFinished");
            return;
        }
    }

    doubleAllowed = false;

    if (activeHand == ActiveHand::MAIN)
    {
        if (auto c = deckPtr->draw())
        {
            player.getMainHand().addCard(std::move(c));
            notify("PlayerCardMain");
        }

        if (player.getMainHand().isBust())
        {
            if (player.hasSplitHand())
            {
                activeHand = ActiveHand::SPLIT;
                notify("SwitchToSplitHand");
            }
            else
            {
                activeHand = ActiveHand::DONE;
                dealerHasHiddenCard = false;
                dealerPlay();
                evaluateHands();
                gameOver = true;
                notify("RoundFinished");
            }
        }
    }
    else if (activeHand == ActiveHand::SPLIT)
    {
        if (auto c = deckPtr->draw())
        {
            player.getSplitHand().addCard(std::move(c));
            notify("PlayerCardSplit");
        }

        if (player.getSplitHand().isBust())
        {
            activeHand = ActiveHand::DONE;
            dealerHasHiddenCard = false;
            dealerPlay();
            evaluateHands();
            gameOver = true;
            notify("RoundFinished");
        }
    }
}

void Game::stand()
{
    if (!roundStarted || gameOver) return;

    if (!player.hasSplitHand())
    {
        if (dealerBlackjack || isBlackjack(player.getMainHand()))
        {
            activeHand = ActiveHand::DONE;
            dealerHasHiddenCard = false;
            dealerPlay();
            evaluateHands();
            gameOver = true;
            notify("RoundFinished");
            return;
        }
    }

    if (activeHand == ActiveHand::MAIN && player.hasSplitHand())
    {
        activeHand = ActiveHand::SPLIT;
        notify("SwitchToSplitHand");
        return;
    }

    activeHand = ActiveHand::DONE;
    dealerHasHiddenCard = false;

    dealerPlay();
    evaluateHands();
    gameOver = true;

    notify("RoundFinished");
}

bool Game::requestSplit()
{
    if (!roundStarted || gameOver) return false;
    if (!splitAllowed) return false;
    if (!player.canSplit()) return false;

    player.performSplit();

    if (auto c1 = deckPtr->draw())
    {
        player.getMainHand().addCard(std::move(c1));
        notify("PlayerCardMain");
    }

    if (auto c2 = deckPtr->draw())
    {
        player.getSplitHand().addCard(std::move(c2));
        notify("PlayerCardSplit");
    }

    splitAllowed = false;
    doubleAllowed = false;

    notify("SplitPerformed");
    return true;
}

bool Game::requestDoubleDown()
{
    if (!roundStarted || gameOver) return false;
    if (!doubleAllowed) return false;

    if (!player.doubleDown()) return false;

    if (activeHand == ActiveHand::MAIN)
    {
        if (auto c = deckPtr->draw())
        {
            player.getMainHand().addCard(std::move(c));
            notify("PlayerCardMain");
        }
    }
    else if (activeHand == ActiveHand::SPLIT)
    {
        if (auto c = deckPtr->draw())
        {
            player.getSplitHand().addCard(std::move(c));
            notify("PlayerCardSplit");
        }
    }

    doubleAllowed = false;
    stand();
    return true;
}

bool Game::requestInsurance()
{
    if (!roundStarted || gameOver) return false;
    if (!insuranceAllowed) return false;

    bool ok = player.takeInsurance();
    if (ok)
    {
        notify("InsuranceTaken");

        if (dealerBlackjack)
        {
            activeHand = ActiveHand::DONE;
            dealerHasHiddenCard = false;
            dealerPlay();
            evaluateHands();
            gameOver = true;
            notify("RoundFinished");
        }
    }
    return ok;
}

void Game::dealInitialCards()
{
    if (auto c1 = deckPtr->draw())
    {
        player.getMainHand().addCard(std::move(c1));
        notify("PlayerCardMain");
    }

    if (auto c2 = deckPtr->draw())
    {
        dealerHand.addCard(std::move(c2));
        notify("DealerCardVisible");
    }

    if (auto c3 = deckPtr->draw())
    {
        player.getMainHand().addCard(std::move(c3));
        notify("PlayerCardMain");
    }

    if (auto c4 = deckPtr->draw())
    {
        dealerHand.addCard(std::move(c4));
        notify("DealerCardHidden");
    }
}

void Game::dealerPlay()
{
    player.resolveInsurance(dealerBlackjack);

    // Use strategy pattern to determine when dealer should hit
    while (dealerStrategy && dealerStrategy->shouldHit(dealerHand))
    {
        if (auto c = deckPtr->draw())
        {
            dealerHand.addCard(std::move(c));
            notify("DealerCardVisible");
        }
        else break;
    }
}

void Game::evaluateHands()
{
    mainResult = EGameResult::None;
    splitResult = EGameResult::None;

    int dealerTotal = dealerHand.getTotal();
    bool dealerBJ = isBlackjack(dealerHand);

    {
        int total = player.getMainHand().getTotal();
        bool playerBJ = isBlackjack(player.getMainHand());

        if (playerBJ && dealerBJ)
        {
            player.push();
            mainResult = EGameResult::Push;
        }
        else if (playerBJ && !dealerBJ)
        {
            player.blackjackWin();
            mainResult = EGameResult::PlayerWon;
        }
        else if (total > 21)
        {
            player.lose();
            mainResult = EGameResult::DealerWon;
        }
        else if (dealerTotal > 21)
        {
            player.win();
            mainResult = EGameResult::PlayerWon;
        }
        else if (total > dealerTotal)
        {
            player.win();
            mainResult = EGameResult::PlayerWon;
        }
        else if (total == dealerTotal)
        {
            player.push();
            mainResult = EGameResult::Push;
        }
        else
        {
            player.lose();
            mainResult = EGameResult::DealerWon;
        }
    }

    if (player.hasSplitHand())
    {
        int splitTotal = player.getSplitHand().getTotal();
        bool splitBJ = isBlackjack(player.getSplitHand());

        int splitBetAmount = player.getCurrentBet() / 2;

        if (splitBJ && dealerBJ)
        {
            player.addToBalance(splitBetAmount);
            splitResult = EGameResult::Push;
        }
        else if (splitBJ && !dealerBJ)
        {
            player.addToBalance(splitBetAmount * 2);
            splitResult = EGameResult::PlayerWon;
        }
        else if (splitTotal > 21)
        {
            splitResult = EGameResult::DealerWon;
        }
        else if (dealerTotal > 21)
        {
            player.addToBalance(splitBetAmount * 2);
            splitResult = EGameResult::PlayerWon;
        }
        else if (splitTotal > dealerTotal)
        {
            player.addToBalance(splitBetAmount * 2);
            splitResult = EGameResult::PlayerWon;
        }
        else if (splitTotal == dealerTotal)
        {
            player.addToBalance(splitBetAmount);
            splitResult = EGameResult::Push;
        }
        else
        {
            splitResult = EGameResult::DealerWon;
        }
    }
}

bool Game::isBlackjack(const IHand& hand) const
{
    return (hand.getCardCount() == 2 && hand.getTotal() == 21);
}

const IHand& Game::getPlayerHand() const
{
    return player.getMainHand();
}

const IHand& Game::getDealerHand() const
{
    return dealerHand;
}

GameState Game::getState() const
{
    GameState s;

    const IHand& main = player.getMainHand();

    s.mainTotal = main.getTotal();

    if (player.hasSplitHand())
    {
        s.splitActive = true;
        s.splitTotal = player.getSplitHand().getTotal();
    }
    else
    {
        s.splitActive = false;
        s.splitTotal = -1;
    }

    if (dealerHasHiddenCard && dealerHand.getCardCount() > 0)
    {
        const ICard* visible = dealerHand.getCard(0);
        s.dealerTotal = static_cast<int>(visible->getValue());
    }
    else
    {
        s.dealerTotal = dealerHand.getTotal();
    }

    s.doubleAllowed = doubleAllowed;
    s.insuranceAllowed = insuranceAllowed;
    s.splitAllowed = splitAllowed;
    s.roundStarted = roundStarted;
    s.gameOver = gameOver;
    s.dealerHidden = dealerHasHiddenCard;

    s.resultMain = mainResult;
    s.resultSplit = splitResult;

    return s;
}

IPlayer& Game::getPlayer()
{
    return player;
}

const IPlayer& Game::getPlayer() const
{
    return player;
}