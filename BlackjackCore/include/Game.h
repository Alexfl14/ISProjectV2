#pragma once

#include "interfaces/IGame.h"
#include "interfaces/IDeck.h"
#include "interfaces/IDealerStrategy.h"
#include "enums/EGameResult.h"
#include "Deck.h"
#include "interfaces/IPlayer.h"
#include "Player.h"
#include "Hand.h"
#include <vector>
#include <string>
#include <memory>
#include "enums/EActiveHand.h"
#include "interfaces/IObserver.h"

class Game : public IGame {
private:
    Deck realDeck;
    IDeck* deckPtr;
    Player player;
    Hand dealerHand;
    std::unique_ptr<IDealerStrategy> dealerStrategy;

    EActiveHand activeHand = EActiveHand::NONE;

    bool roundStarted = false;
    bool gameOver = false;

    bool splitAllowed = false;
    bool insuranceAllowed = false;
    bool doubleAllowed = false;

    bool dealerHasHiddenCard = false;
    bool dealerBlackjack = false;

    EGameResult mainResult = EGameResult::None;
    EGameResult splitResult = EGameResult::None;

    std::vector<IObserver*> observers;

    void notify(const std::string& event);
    void dealInitialCards();
    void dealerPlay();
    void evaluateHands();
    bool isBlackjack(const IHand& hand) const;

public:
    explicit Game(int initialBalance = 1000, int numberOfDecks = 6, bool autoReshuffle = true);

    void forceDeck(IDeck* fake);
    void setDealerStrategy(std::unique_ptr<IDealerStrategy> strategy);

    // New methods to control deck randomness
    void setNumberOfDecks(int numDecks);
    int getNumberOfDecks() const;
    void setAutoReshuffle(bool enable);
    bool getAutoReshuffle() const;
    IDeck& getDeck();

    void addObserver(IObserver* observer) override;
    void removeObserver(IObserver* observer) override;
    void notifyObservers(const std::string& eventName) override;

    void startGame() override;
    void hit() override;
    void stand() override;
    void reset() override;

    bool requestBet(int amount) override;
    bool requestClearBet() override;
    bool requestSplit() override;
    bool requestDoubleDown() override;
    bool requestInsurance() override;

    const IHand& getPlayerHand() const override;
    const IHand& getDealerHand() const override;
    GameState getState() const override;

    IPlayer& getPlayer();
    const IPlayer& getPlayer() const;
};
