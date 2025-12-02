#pragma once

#include "ICard.h"
#include <cstddef>
#include <memory>

class IHand {
public:
    virtual ~IHand() = default;

    virtual std::size_t getCardCount() const = 0;

    virtual const ICard* getCard(std::size_t index) const = 0;

    virtual void addCard(std::unique_ptr<ICard> card) = 0;

    virtual void removeCardAt(std::size_t index) = 0;

    virtual void clear() = 0;

    virtual int getTotal() const = 0;

    virtual bool isBust() const = 0;
};
