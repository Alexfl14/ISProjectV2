#include "Hand.h"

std::size_t Hand::getCardCount() const {
    return cards.size();
}

const ICard* Hand::getCard(std::size_t index) const {
    if (index >= cards.size()) {
        return nullptr;
    }
    return cards[index].get();
}

void Hand::addCard(std::unique_ptr<ICard> card) {
    if (card) {
        cards.push_back(std::move(card));
    }
}

void Hand::removeCardAt(std::size_t index) {
    if (index < cards.size()) {
        cards.erase(cards.begin() + static_cast<std::ptrdiff_t>(index));
    }
}

void Hand::clear() {
    cards.clear();
}

int Hand::getTotal() const {
    int total = 0;
    int aceCount = 0;

    for (const auto& card : cards) {
        int value = static_cast<int>(card->getValue());
        total += value;

        if (card->getValue() == ECardValue::Ace) {
            ++aceCount;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        --aceCount;
    }

    return total;
}

bool Hand::isBust() const {
    return getTotal() > 21;
}
