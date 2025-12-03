#pragma once

class IHand;
class IDeck;

class IDealerStrategy {
public:
    virtual ~IDealerStrategy() = default;

    
    virtual bool shouldHit(const IHand& dealerHand) const = 0;

    virtual const char* getStrategyName() const = 0;
};
