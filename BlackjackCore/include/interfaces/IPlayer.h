#pragma once
#include "IHand.h"

class IPlayer {
public:
    virtual ~IPlayer() = default;

    virtual int getBalance() const = 0;
    virtual int getCurrentBet() const = 0;
    virtual int getInsuranceBet() const = 0;

    virtual void addToBalance(int amount) = 0;

    virtual bool bet(int amount) = 0;
    virtual void clearBet() = 0;
    virtual bool takeInsurance() = 0;
    virtual bool doubleDown() = 0;

    virtual bool canSplit() const = 0;
    virtual void performSplit() = 0;
    virtual bool hasSplitHand() const = 0;

    virtual IHand& getMainHand() = 0;
    virtual IHand& getSplitHand() = 0;
    virtual const IHand& getMainHand() const = 0;
    virtual const IHand& getSplitHand() const = 0;

    virtual void win() = 0;
    virtual void blackjackWin() = 0;
    virtual void push() = 0;
    virtual void lose() = 0;
    virtual void resolveInsurance(bool dealerHasBlackjack) = 0;

    virtual void resetForNewRound() = 0;
};