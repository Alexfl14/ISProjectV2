#pragma once

#include "interfaces/IDealerStrategy.h"

class StandardDealerStrategy : public IDealerStrategy {
public:
    bool shouldHit(const IHand& dealerHand) const override;
    const char* getStrategyName() const override;
};
