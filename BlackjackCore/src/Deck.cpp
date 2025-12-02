#include "Deck.h"

Deck::Deck() {
    cards.reserve(52);

    for (int s = 0; s < 4; ++s) {
        for (int v = 2; v <= 14; ++v) {

            ECardValue val;

            if (v <= 10) val = static_cast<ECardValue>(v);
            else if (v == 11) val = ECardValue::Jack;
            else if (v == 12) val = ECardValue::Queen;
            else if (v == 13) val = ECardValue::King;
            else              val = ECardValue::Ace;

            cards.push_back(std::make_unique<Card>(
                static_cast<ESuit>(s),
                val
            ));
        }
    }
}

void Deck::shuffle() {
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

std::unique_ptr<ICard> Deck::draw() {
    if (cards.empty()) return nullptr;

    auto card = std::move(cards.back());
    cards.pop_back();
    return card;
}

int Deck::size() const {
    return static_cast<int>(cards.size());
}
