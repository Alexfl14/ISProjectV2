#pragma once

#include "IHand.h"
#include "IObserver.h"
#include "GameState.h"

class IGame : public ISubject {
public:
    virtual ~IGame() = default;

    virtual void startGame() = 0;   
    virtual void hit() = 0;         
    virtual void stand() = 0;       
    virtual void reset() = 0;      

    virtual bool requestBet(int amount) = 0;      
    virtual bool requestClearBet() = 0;           
    virtual bool requestSplit() = 0;              
    virtual bool requestDoubleDown() = 0;         
    virtual bool requestInsurance() = 0;          

    virtual const IHand& getPlayerHand() const = 0;   
    virtual const IHand& getDealerHand() const = 0;

    virtual GameState getState() const = 0;       
};
