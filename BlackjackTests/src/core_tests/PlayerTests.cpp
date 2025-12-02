#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Player.h"
#include "Hand.h"
#include "interfaces/IHand.h"
#include "interfaces/ICard.h"
#include "Card.h"
#include "enums/ESuit.h"
#include "enums/ECardValue.h"


static std::unique_ptr<ICard> Mk(ESuit s, ECardValue v) {
    return std::make_unique<Card>(s, v);
}


TEST(PlayerTests, BetSubtractsFromBalanceAndStoresCurrentBet) {
    Player p(1000);
    EXPECT_TRUE(p.bet(200));
    EXPECT_EQ(p.getBalance(), 800);
    EXPECT_EQ(p.getCurrentBet(), 200);
}

TEST(PlayerTests, BetFailsIfNotEnoughBalance) {
    Player p(100);
    EXPECT_FALSE(p.bet(200));
    EXPECT_EQ(p.getBalance(), 100);
    EXPECT_EQ(p.getCurrentBet(), 0);
}

TEST(PlayerTests, ChangingBetRefundsPreviousBetCorrectly) {
    Player p(1000);
    ASSERT_TRUE(p.bet(300));
    ASSERT_TRUE(p.bet(200));

    EXPECT_EQ(p.getBalance(), 800);
    EXPECT_EQ(p.getCurrentBet(), 200);
}

TEST(PlayerTests, ClearBetRestoresBalance) {
    Player p(500);
    ASSERT_TRUE(p.bet(200));

    p.clearBet();
    EXPECT_EQ(p.getBalance(), 500);
    EXPECT_EQ(p.getCurrentBet(), 0);
}


TEST(PlayerTests, DoubleDownDoublesBetAndReducesBalance) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));
    ASSERT_TRUE(p.doubleDown());

    EXPECT_EQ(p.getCurrentBet(), 400);
    EXPECT_EQ(p.getBalance(), 600);
}

TEST(PlayerTests, DoubleDownFailsWhenNotEnoughMoney) {
    Player p(300);
    ASSERT_TRUE(p.bet(200));

    EXPECT_FALSE(p.doubleDown());
    EXPECT_EQ(p.getBalance(), 100);
    EXPECT_EQ(p.getCurrentBet(), 200);
}


TEST(PlayerTests, InsuranceTakesHalfOfCurrentBet) {
    Player p(1000);
    ASSERT_TRUE(p.bet(300));

    ASSERT_TRUE(p.takeInsurance());
    EXPECT_EQ(p.getInsuranceBet(), 150);
    EXPECT_EQ(p.getBalance(), 550);
}

TEST(PlayerTests, InsuranceFailsWhenNoBetPlaced) {
    Player p(1000);
    EXPECT_FALSE(p.takeInsurance());
    EXPECT_EQ(p.getInsuranceBet(), 0);
}

TEST(PlayerTests, InsuranceResolvesCorrectlyWhenDealerHasBlackjack) {
    Player p(1000);
    ASSERT_TRUE(p.bet(300));
    ASSERT_TRUE(p.takeInsurance());

    p.resolveInsurance(true);

    EXPECT_EQ(p.getBalance(), 850);
}

TEST(PlayerTests, InsuranceLosesWhenDealerHasNoBlackjack) {
    Player p(1000);
    ASSERT_TRUE(p.bet(300));
    ASSERT_TRUE(p.takeInsurance());

    p.resolveInsurance(false);

    EXPECT_EQ(p.getBalance(), 550);
}




TEST(PlayerTests, WinAddsMoney) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.win();
    EXPECT_EQ(p.getBalance(), 1200);
}

TEST(PlayerTests, BlackjackWinAdds150Percent) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.blackjackWin();
    EXPECT_EQ(p.getBalance(), 1300);
}

TEST(PlayerTests, LoseSubtractsBet) {
    Player p(1000);

    ASSERT_TRUE(p.bet(200));

    p.lose();

    EXPECT_EQ(p.getBalance(), 800);
}


TEST(PlayerTests, PushRefundsBet) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.push();
    EXPECT_EQ(p.getBalance(), 1000);
}


TEST(PlayerTests, CanSplitWhenValuesMatch) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.getMainHand().addCard(Mk(ESuit::Hearts, ECardValue::Eight));
    p.getMainHand().addCard(Mk(ESuit::Spades, ECardValue::Eight));

    EXPECT_TRUE(p.canSplit());
}

TEST(PlayerTests, CannotSplitWhenValuesDoNotMatch) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.getMainHand().addCard(Mk(ESuit::Hearts, ECardValue::Eight));
    p.getMainHand().addCard(Mk(ESuit::Spades, ECardValue::Nine));

    EXPECT_FALSE(p.canSplit());
}

TEST(PlayerTests, PerformSplitMovesCardToSplitHandAndDoublesBet) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));

    p.getMainHand().addCard(Mk(ESuit::Clubs, ECardValue::King));
    p.getMainHand().addCard(Mk(ESuit::Clubs, ECardValue::King));

    ASSERT_TRUE(p.canSplit());
    p.performSplit();

    EXPECT_TRUE(p.hasSplitHand());
    EXPECT_EQ(p.getMainHand().getCardCount(), 1u);
    EXPECT_EQ(p.getSplitHand().getCardCount(), 1u);

    EXPECT_EQ(p.getCurrentBet(), 400);
    EXPECT_EQ(p.getBalance(), 600);
}

TEST(PlayerTests, SplitFailsIfNotEnoughMoney) {
    Player p(300);
    ASSERT_TRUE(p.bet(200));

    p.getMainHand().addCard(Mk(ESuit::Clubs, ECardValue::Queen));
    p.getMainHand().addCard(Mk(ESuit::Diamonds, ECardValue::Queen));

    EXPECT_FALSE(p.canSplit());
}


TEST(PlayerTests, ResetForNewRoundClearsHandsAndInsuranceAndBet) {
    Player p(1000);
    ASSERT_TRUE(p.bet(200));
    ASSERT_TRUE(p.takeInsurance());
    p.getMainHand().addCard(Mk(ESuit::Hearts, ECardValue::Ten));

    p.resetForNewRound();

    EXPECT_EQ(p.getCurrentBet(), 0);
    EXPECT_EQ(p.getInsuranceBet(), 0);
    EXPECT_EQ(p.hasSplitHand(), false);
    EXPECT_EQ(p.getMainHand().getCardCount(), 0u);
}
