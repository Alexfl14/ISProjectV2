#pragma once

#include "enums/ESuit.h"
#include "enums/ECardValue.h"

class ICard {
public:
    virtual ~ICard() = default;

    virtual ESuit getSuit() const = 0;
    virtual ECardValue getValue() const = 0;

    virtual bool isHidden() const = 0;
    virtual void reveal() = 0;

    virtual ICard* clone() const = 0;
};
