#include <gtest/gtest.h>
#include "interfaces/ICard.h"
#include "Card.h"
#include "enums/ESuit.h"
#include "enums/ECardValue.h"

TEST(CardTests, CardStoresCorrectSuitAndValue) {
    Card c(ESuit::Hearts, ECardValue::Queen);
    EXPECT_EQ(c.getSuit(), ESuit::Hearts);
    EXPECT_EQ(c.getValue(), ECardValue::Queen);
}

TEST(CardTests, HiddenCardRevealsProperly) {
    Card c(ESuit::Spades, ECardValue::King, true);
    EXPECT_TRUE(c.isHidden());
    c.reveal();
    EXPECT_FALSE(c.isHidden());
}

TEST(CardTests, CloneCreatesIdenticalCard) {
    Card c(ESuit::Clubs, ECardValue::Ten);
    std::unique_ptr<ICard> clone(c.clone());
    EXPECT_EQ(clone->getValue(), ECardValue::Ten);
    EXPECT_EQ(clone->getSuit(), ESuit::Clubs);
}
