#pragma once
#include "interfaces/IPlayer.h"
#include "Hand.h"

class Player : public IPlayer {
private:
    int balance;
    int currentBet;
    int insuranceBet;

    Hand mainHand;
    Hand splitHand;
    bool splitActive = false;

public:
    explicit Player(int initialBalance);

    int getBalance() const override;
    int getCurrentBet() const override;
    int getInsuranceBet() const override;

    void addToBalance(int amount) override;

    bool bet(int amount) override;
    void clearBet() override;
    bool takeInsurance() override;
    bool doubleDown() override;

    bool canSplit() const override;
    void performSplit() override;
    bool hasSplitHand() const override;

    IHand& getMainHand() override;
    IHand& getSplitHand() override;
    const IHand& getMainHand() const override;
    const IHand& getSplitHand() const override;

    void win() override;
    void blackjackWin() override;
    void push() override;
    void lose() override;
    void resolveInsurance(bool dealerHasBlackjack) override;

    void resetForNewRound() override;
};