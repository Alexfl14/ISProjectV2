#pragma once

#include "interfaces/IDeck.h"
#include "interfaces/ICard.h"
#include <vector>
#include <memory>

class FakeDeck : public IDeck
{
private:
    std::vector<std::unique_ptr<ICard>> cards;

public:
    FakeDeck() = default;

    void pushFront(std::unique_ptr<ICard> c)
    {
        cards.insert(cards.begin(), std::move(c));
    }

    void pushBack(std::unique_ptr<ICard> c)
    {
        cards.push_back(std::move(c));
    }

    void clear()
    {
        cards.clear();
    }

    std::unique_ptr<ICard> draw() override
    {
        if (cards.empty()) return nullptr;
        auto c = std::move(cards.front());
        cards.erase(cards.begin());
        return c;
    }

    void shuffle() override
    {
    }

    int size() const override
    {
        return static_cast<int>(cards.size());
    }
};