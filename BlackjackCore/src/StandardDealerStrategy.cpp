#include "StandardDealerStrategy.h"
#include "interfaces/IHand.h"

bool StandardDealerStrategy::shouldHit(const IHand& dealerHand) const
{
    return dealerHand.getTotal() < 17;
}

const char* StandardDealerStrategy::getStrategyName() const
{
    return "Standard (Stand on 17)";
}
