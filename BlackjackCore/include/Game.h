#pragma once

#include "interfaces/IGame.h"
#include "interfaces/IDeck.h"
#include "enums/EGameResult.h"
#include "Deck.h"
#include "interfaces/IPlayer.h"
#include "Player.h"
#include "Hand.h"
#include <vector>
#include <string>

class Game : public IGame {
private:
    IDeck* deckPtr;
    Deck realDeck;
    Player player;
    Hand dealerHand;

    enum class ActiveHand { NONE, MAIN, SPLIT, DONE };
    ActiveHand activeHand = ActiveHand::NONE;

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
    explicit Game(int initialBalance = 1000);

    void forceDeck(IDeck* fake);

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
