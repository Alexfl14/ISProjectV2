#pragma once

#include "interfaces/IDealerStrategy.h"

class HitOnSoft17Strategy : public IDealerStrategy {
public:
    bool shouldHit(const IHand& dealerHand) const override;
    const char* getStrategyName() const override;

private:
    bool isSoft17(const IHand& dealerHand) const;
};
