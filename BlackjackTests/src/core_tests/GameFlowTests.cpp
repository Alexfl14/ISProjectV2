#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Game.h"
#include "FakeDeck.h"
#include "Card.h"
#include "enums/ESuit.h"
#include "enums/ECardValue.h"
#include "enums/EGameResult.h"

static std::unique_ptr<ICard> C(ESuit s, ECardValue v)
{
    return std::make_unique<Card>(s, v);
}

static void addFront(FakeDeck* fd, ESuit s, ECardValue v)
{
    fd->pushFront(std::make_unique<Card>(s, v));
}

TEST(GameFlow, StartGameDealsCorrectCards)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Hearts, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Spades, ECardValue::King);
    addFront(fd, ESuit::Diamonds, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    EXPECT_EQ(game.getPlayerHand().getCardCount(), 2u);
    EXPECT_EQ(game.getDealerHand().getCardCount(), 2u);
    EXPECT_EQ(game.getState().mainTotal, 8 + 5);
    EXPECT_EQ(game.getState().dealerHidden, true);
}

TEST(GameFlow, HitGivesPlayerOneCard)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Hearts, ECardValue::Three);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(100);
    game.startGame();
    game.hit();

    EXPECT_EQ(game.getPlayerHand().getCardCount(), 3u);
}

TEST(GameFlow, PlayerBustEndsRound)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Hearts, ECardValue::King);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(100);
    game.startGame();
    game.hit();

    EXPECT_TRUE(game.getState().gameOver);
}

TEST(GameFlow, DealerBustMakesPlayerWin)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Spades, ECardValue::Nine);
    addFront(fd, ESuit::Clubs, ECardValue::Nine);
    addFront(fd, ESuit::Hearts, ECardValue::Six);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getState().resultMain, EGameResult::PlayerWon);
}

TEST(GameFlow, PlayerHasBlackjack)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Ace);
    addFront(fd, ESuit::Clubs, ECardValue::Nine);
    addFront(fd, ESuit::Diamonds, ECardValue::King);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getState().resultMain, EGameResult::PlayerWon);
}

TEST(GameFlow, PlayerAndDealerBlackjackPush)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Ten);
    addFront(fd, ESuit::Hearts, ECardValue::Ace);
    addFront(fd, ESuit::Diamonds, ECardValue::Ace);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();
    game.stand();

    GameState s = game.getState();

    EXPECT_TRUE(s.gameOver);
    EXPECT_EQ(s.resultMain, EGameResult::Push);
    EXPECT_EQ(game.getPlayer().getBalance(), 1000);
}

TEST(GameFlow, InsuranceAllowedOnlyIfDealerShowsAce)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Hearts, ECardValue::Ace);
    addFront(fd, ESuit::Diamonds, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    EXPECT_TRUE(game.getState().insuranceAllowed);
}

TEST(GameFlow, DoubleDownWorksAndGivesOneCard)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::Three);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(100);
    game.startGame();

    EXPECT_TRUE(game.requestDoubleDown());
    EXPECT_EQ(game.getPlayerHand().getCardCount(), 3u);
}

TEST(GameFlow, SplitCreatesTwoHands)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Spades, ECardValue::Seven);
    addFront(fd, ESuit::Hearts, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Hearts, ECardValue::Eight);
    addFront(fd, ESuit::Diamonds, ECardValue::King);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    EXPECT_TRUE(game.requestSplit());
    EXPECT_EQ(game.getPlayer().getMainHand().getCardCount(), 2u);
    EXPECT_EQ(game.getPlayer().getSplitHand().getCardCount(), 2u);
}

TEST(GameFlow, InsurancePaysIfDealerHasBlackjack)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::King);
    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Hearts, ECardValue::Ace);
    addFront(fd, ESuit::Diamonds, ECardValue::Nine);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    ASSERT_TRUE(game.requestInsurance());

    GameState s = game.getState();

    EXPECT_TRUE(s.gameOver);
    EXPECT_EQ(game.getPlayer().getBalance(), 900);
}

TEST(GameFlow, InsuranceLostIfDealerHasNoBlackjack)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::Nine);
    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Hearts, ECardValue::Six);
    addFront(fd, ESuit::Diamonds, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    EXPECT_FALSE(game.requestInsurance());
}

TEST(GameFlow, DoubleDownPlayerBustsAndRoundEnds)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Hearts, ECardValue::King);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Hearts, ECardValue::Five);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    EXPECT_TRUE(game.requestDoubleDown());
    EXPECT_TRUE(game.getState().gameOver);
}

TEST(GameFlow, SplitLoseBothHands)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::King);
    addFront(fd, ESuit::Spades, ECardValue::King);
    addFront(fd, ESuit::Hearts, ECardValue::Five);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);
    addFront(fd, ESuit::Clubs, ECardValue::Ten);
    addFront(fd, ESuit::Hearts, ECardValue::Eight);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);
    addFront(fd, ESuit::Spades, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    ASSERT_TRUE(game.requestSplit());

    game.hit();
    game.hit();

    GameState s = game.getState();

    EXPECT_TRUE(s.gameOver);
    EXPECT_EQ(s.resultMain, EGameResult::DealerWon);
    EXPECT_EQ(s.resultSplit, EGameResult::DealerWon);
}

TEST(GameFlow, SplitWinFirstLoseSecond)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Hearts, ECardValue::Eight);
    addFront(fd, ESuit::Clubs, ECardValue::Ace);
    addFront(fd, ESuit::Spades, ECardValue::Three);
    addFront(fd, ESuit::Clubs, ECardValue::Nine);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Hearts, ECardValue::Eight);
    addFront(fd, ESuit::Diamonds, ECardValue::Five);
    addFront(fd, ESuit::Spades, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();

    ASSERT_TRUE(game.requestSplit());

    game.hit();
    game.stand();
    game.hit();
    game.stand();

    GameState s = game.getState();

    EXPECT_EQ(s.resultMain, EGameResult::PlayerWon);
    EXPECT_EQ(s.resultSplit, EGameResult::DealerWon);
}

TEST(GameFlow, HitSwitchesToSplitAfterMainBusts)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::Nine);
    addFront(fd, ESuit::Hearts, ECardValue::King);
    addFront(fd, ESuit::Spades, ECardValue::Eight);
    addFront(fd, ESuit::Clubs, ECardValue::Eight);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);
    addFront(fd, ESuit::Hearts, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();
    game.requestSplit();
    game.hit();

    EXPECT_EQ(game.getState().splitActive, true);
}

TEST(GameFlow, StandOnMainSwitchesToSplitHand)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Clubs, ECardValue::Five);
    addFront(fd, ESuit::Spades, ECardValue::Six);
    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Hearts, ECardValue::Eight);
    addFront(fd, ESuit::Clubs, ECardValue::Seven);
    addFront(fd, ESuit::Diamonds, ECardValue::Eight);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();
    game.requestSplit();
    game.stand();

    EXPECT_TRUE(game.getState().splitActive);
}

TEST(GameFlow, ResetClearsAllState)
{
    FakeDeck* fd = new FakeDeck();

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(200);
    game.startGame();
    game.hit();
    game.reset();

    GameState s = game.getState();

    EXPECT_FALSE(s.roundStarted);
    EXPECT_EQ(s.mainTotal, 0);
    EXPECT_EQ(game.getPlayer().getMainHand().getCardCount(), 0u);
    EXPECT_EQ(game.getDealerHand().getCardCount(), 0u);
    EXPECT_EQ(game.getPlayer().getCurrentBet(), 0);
}

TEST(GameFlow, HitDisablesDoubleDown)
{
    FakeDeck* fd = new FakeDeck();

    addFront(fd, ESuit::Spades, ECardValue::Ten);
    addFront(fd, ESuit::Clubs, ECardValue::Six);
    addFront(fd, ESuit::Hearts, ECardValue::Five);
    addFront(fd, ESuit::Diamonds, ECardValue::Seven);

    Game game(1000);
    game.forceDeck(fd);

    game.requestBet(100);
    game.startGame();
    game.hit();

    EXPECT_FALSE(game.getState().doubleAllowed);
}