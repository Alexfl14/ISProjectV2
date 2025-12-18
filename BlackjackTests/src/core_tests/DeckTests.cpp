#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Deck.h"
#include "Card.h"
#include "enums/ESuit.h"
#include "enums/ECardValue.h"

TEST(DeckTests, DeckStartsWith52Cards) {
    Deck d;
    EXPECT_EQ(d.size(), 52);
}

TEST(DeckTests, DrawReducesDeckSize) {
    Deck d;
    auto c1 = d.draw();
    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(d.size(), 51);
}

TEST(DeckTests, DrawReturnsNullOnEmptyDeck) {
    Deck d;

    for (int i = 0; i < 52; i++) {
        d.draw();
    }

    EXPECT_NE(d.draw(), nullptr);
}

TEST(DeckTests, ShuffleDoesNotCrash) {
    Deck d;
    EXPECT_NO_THROW(d.shuffle());
}
