#pragma once

#include "interfaces/IDeck.h"
#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>

class Deck : public IDeck {
private:
    std::vector<std::unique_ptr<ICard>> cards;

public:
    Deck();
    void shuffle() override;
    std::unique_ptr<ICard> draw() override;
    int size() const override;
};
