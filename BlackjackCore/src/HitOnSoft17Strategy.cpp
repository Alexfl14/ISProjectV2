#include "HitOnSoft17Strategy.h"
#include "interfaces/IHand.h"
#include "interfaces/ICard.h"
#include "enums/ECardValue.h"

bool HitOnSoft17Strategy::shouldHit(const IHand& dealerHand) const
{
    int total = dealerHand.getTotal();
    
    if (total < 17) {
        return true;
    }
    
    if (total > 17) {
        return false;
    }
    
    if (total == 17 && isSoft17(dealerHand)) {
        return true;
    }
    
    return false;
}

bool HitOnSoft17Strategy::isSoft17(const IHand& dealerHand) const
{
    if (dealerHand.getTotal() != 17) {
        return false;
    }
    
    bool hasAce = false;
    int totalWithoutAce = 0;
    
    for (std::size_t i = 0; i < dealerHand.getCardCount(); ++i) {
        const ICard* card = dealerHand.getCard(i);
        if (card) {
            if (card->getValue() == ECardValue::Ace) {
                hasAce = true;
                totalWithoutAce += 1; // Count ace as 1
            } else {
                totalWithoutAce += static_cast<int>(card->getValue());
            }
        }
    }
    
    return hasAce && (totalWithoutAce + 10 == 17);
}

const char* HitOnSoft17Strategy::getStrategyName() const
{
    return "Hit on Soft 17";
}
