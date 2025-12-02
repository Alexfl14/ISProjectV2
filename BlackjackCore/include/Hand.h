#pragma once

#include "interfaces/IHand.h"
#include <vector>
#include <memory>

class Hand : public IHand {
private:
    std::vector<std::unique_ptr<ICard>> cards;

public:
    Hand() = default;
    ~Hand() override = default;

    std::size_t getCardCount() const override;
    const ICard* getCard(std::size_t index) const override;

    void addCard(std::unique_ptr<ICard> card) override;
    void removeCardAt(std::size_t index) override;
    void clear() override;

    int getTotal() const override;
    bool isBust() const override;
};
