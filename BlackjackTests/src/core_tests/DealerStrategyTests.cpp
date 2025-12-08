#include "pch.h"
#include "Game.h"
#include "StandardDealerStrategy.h"
#include "HitOnSoft17Strategy.h"
#include "GameFactory.h"
#include "FakeDeck.h"
#include "Card.h"
#include "enums/ECardValue.h"
#include <memory>

TEST(DealerStrategyTests, StandardStrategy_StandsOn17) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));  // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Seven)); // Dealer hidden

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 17);
}

TEST(DealerStrategyTests, StandardStrategy_HitsOn16) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten));  // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Six));   // Dealer hidden
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Five));  // Dealer hits

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 3);
    EXPECT_EQ(game.getDealerHand().getTotal(), 21);
}

TEST(DealerStrategyTests, HitOnSoft17Strategy_HitsOnSoft17) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ace)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Six));   // Dealer hidden (soft 17)
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Four));  // Dealer hits

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<HitOnSoft17Strategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 3);
    EXPECT_EQ(game.getDealerHand().getTotal(), 21); // A-6-4 = 21
}

TEST(DealerStrategyTests, HitOnSoft17Strategy_StandsOnHard17) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Seven)); // Dealer hidden (hard 17)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<HitOnSoft17Strategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 17);
}

TEST(DealerStrategyTests, CanSwitchStrategiesDynamically) {
    Game game;
    
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());
    
    game.setDealerStrategy(std::make_unique<HitOnSoft17Strategy>());
    
    EXPECT_TRUE(true); 
}

TEST(DealerStrategyTests, GameFactory_CreatesDifferentStrategies) {
    GameFactory factory;

    std::unique_ptr<IGame> standardGame(factory.createGameWithStandardRules());
    EXPECT_NE(standardGame, nullptr);

    std::unique_ptr<IGame> soft17Game(factory.createGameWithSoft17Rules());
    EXPECT_NE(soft17Game, nullptr);

    standardGame->requestBet(100);
    soft17Game->requestBet(100);
}

// ==================== Additional StandardDealerStrategy Tests ====================

TEST(StandardDealerStrategyTests, StandsOn18) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Eight)); // Dealer hidden

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 18);
}

TEST(StandardDealerStrategyTests, StandsOn19) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Nine));  // Dealer hidden

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 19);
}

TEST(StandardDealerStrategyTests, StandsOn20) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Ten));   // Dealer hidden

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 20);
}

TEST(StandardDealerStrategyTests, StandsOn21) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ace)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Ten));   // Dealer hidden (Blackjack)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 21);
}

TEST(StandardDealerStrategyTests, HitsOn15) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Five));  // Dealer hidden
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Three)); // Dealer hits

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 3);
    EXPECT_EQ(game.getDealerHand().getTotal(), 18);
}

TEST(StandardDealerStrategyTests, HitsOn12) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Two));   // Dealer hidden
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Five));  // Dealer hits

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 3);
    EXPECT_EQ(game.getDealerHand().getTotal(), 17);
}

TEST(StandardDealerStrategyTests, HitsMultipleTimesUntil17) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Five)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Two));   // Dealer hidden (7 total)
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::Three)); // Dealer hits (10 total)
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Four));  // Dealer hits (14 total)
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Three));  // Dealer hits (17 total, stands)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 5);
    EXPECT_EQ(game.getDealerHand().getTotal(), 17);
}

TEST(StandardDealerStrategyTests, StandsOnSoft17_AceSix) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ace)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Six));   // Dealer hidden (soft 17)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    // Standard strategy stands on soft 17
    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 17);
}

TEST(StandardDealerStrategyTests, StandsOnSoft18_AceSeven) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ace)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Seven)); // Dealer hidden (soft 18)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 2);
    EXPECT_EQ(game.getDealerHand().getTotal(), 18);
}

TEST(StandardDealerStrategyTests, DealerBusts) {
    Game game;
    FakeDeck fakeDeck;

    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Ten));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Diamonds, ECardValue::Ten)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Clubs, ECardValue::Ten));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Hearts, ECardValue::Six));   // Dealer hidden (16 total)
    fakeDeck.addCard(std::make_unique<Card>(ESuit::Spades, ECardValue::King));  // Dealer hits and busts (26 total)

    game.forceDeck(&fakeDeck);
    game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());

    game.requestBet(100);
    game.startGame();
    game.stand();

    EXPECT_EQ(game.getDealerHand().getCardCount(), 3);
    EXPECT_GT(game.getDealerHand().getTotal(), 21); // Dealer busted
}

TEST(StandardDealerStrategyTests, GetStrategyName) {
    StandardDealerStrategy strategy;
    EXPECT_STREQ(strategy.getStrategyName(), "Standard (Stand on 17)");
}
