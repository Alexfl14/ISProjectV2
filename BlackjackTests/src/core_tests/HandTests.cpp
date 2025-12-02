#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Hand.h"
#include "interfaces/IHand.h"
#include "interfaces/ICard.h"
#include "Card.h"
#include "enums/ESuit.h"
#include "enums/ECardValue.h"


static std::unique_ptr<ICard> Mk(ESuit s, ECardValue v) {
    return std::make_unique<Card>(s, v);
}

TEST(HandTests, NewlyCreatedHandIsEmpty) {
    Hand h;
    EXPECT_EQ(h.getCardCount(), 0u);
    EXPECT_EQ(h.getTotal(), 0);
    EXPECT_FALSE(h.isBust());
}

TEST(HandTests, AddCardIncreasesCountAndAccessibleByGetCard) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Five));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Seven));

    EXPECT_EQ(h.getCardCount(), 2u);

    const ICard* c1 = h.getCard(0);
    const ICard* c2 = h.getCard(1);

    ASSERT_NE(c1, nullptr);
    ASSERT_NE(c2, nullptr);

    EXPECT_EQ(c1->getValue(), ECardValue::Five);
    EXPECT_EQ(c2->getValue(), ECardValue::Seven);
}


TEST(HandTests, TotalWithoutAcesIsSumOfValues) {
    Hand h;
    h.addCard(Mk(ESuit::Spades, ECardValue::Ten));
    h.addCard(Mk(ESuit::Diamonds, ECardValue::Nine));
    EXPECT_EQ(h.getTotal(), 19);
    EXPECT_FALSE(h.isBust());
}


TEST(HandTests, SingleAceCountsAs11IfNotBusting) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ace));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Nine));
    EXPECT_EQ(h.getTotal(), 20);
}

TEST(HandTests, AceFallsBackTo1ToAvoidBust) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ace));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Nine));
    h.addCard(Mk(ESuit::Spades, ECardValue::Queen));
    EXPECT_EQ(h.getTotal(), 20);
}


TEST(HandTests, TwoAcesTotalCorrect) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ace));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Ace));
    EXPECT_EQ(h.getTotal(), 12); \
}


TEST(HandTests, AcesAndNineMakeTwentyOne) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ace));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Ace));
    h.addCard(Mk(ESuit::Spades, ECardValue::Nine));
    EXPECT_EQ(h.getTotal(), 21);
}


TEST(HandTests, HandIsBustWhenTotalOver21) {
    Hand h;
    h.addCard(Mk(ESuit::Diamonds, ECardValue::Ten));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Queen));
    h.addCard(Mk(ESuit::Spades, ECardValue::Five));
    EXPECT_TRUE(h.isBust());
}


TEST(HandTests, ClearResetsTheHand) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ten));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Two));

    h.clear();
    EXPECT_EQ(h.getCardCount(), 0u);
    EXPECT_EQ(h.getTotal(), 0);
}


TEST(HandTests, RemoveCardAtWorksCorrectly) {
    Hand h;
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ten));
    h.addCard(Mk(ESuit::Spades, ECardValue::Four));
    h.addCard(Mk(ESuit::Clubs, ECardValue::Three));

    h.removeCardAt(1); 
    EXPECT_EQ(h.getCardCount(), 2u);

    EXPECT_EQ(h.getCard(0)->getValue(), ECardValue::Ten);
    EXPECT_EQ(h.getCard(1)->getValue(), ECardValue::Three);
}


TEST(HandTests, GetCardOutOfRangeReturnsNull) {
    Hand h;
    EXPECT_EQ(h.getCard(0), nullptr);
    h.addCard(Mk(ESuit::Hearts, ECardValue::Ten));
    EXPECT_EQ(h.getCard(5), nullptr);
}
