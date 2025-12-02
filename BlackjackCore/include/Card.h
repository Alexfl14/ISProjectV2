#pragma once

#include "interfaces/ICard.h"

class Card : public ICard {
private:
    ESuit suit;
    ECardValue value;
    bool faceDown;

public:
    Card(ESuit s, ECardValue v, bool hidden = false)
        : suit(s), value(v), faceDown(hidden) {
    }

    ESuit getSuit() const override { return suit; }
    ECardValue getValue() const override { return value; }

    bool isHidden() const override { return faceDown; }
    void reveal() override { faceDown = false; }

    ICard* clone() const override {
        return new Card(suit, value, faceDown);
    }
};
