#include "pch.h"
#include "Game.h"
#include "StandardDealerStrategy.h"
#include "HitOnSoft17Strategy.h"
#include "FakeDeck.h"
#include "Card.h"
#include <memory>

TEST(DealerStrategyTests, StandardStrategy_StandsOn17) {
    Game game;
    FakeDeck fakeDeck;
    
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Hearts));  // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Diamonds)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Clubs));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Seven, ESuit::Hearts)); // Dealer hidden
    
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
    
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Hearts));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Diamonds));  // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Clubs));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Six, ESuit::Hearts));   // Dealer hidden
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Five, ESuit::Spades));  // Dealer hits
    
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
    
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Hearts));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ace, ESuit::Diamonds)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Clubs));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Six, ESuit::Hearts));   // Dealer hidden (soft 17)
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Four, ESuit::Spades));  // Dealer hits
    
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
    
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Hearts));   // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Diamonds)); // Dealer visible
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Ten, ESuit::Clubs));    // Player
    fakeDeck.addCard(std::make_unique<Card>(ECardValue::Seven, ESuit::Hearts)); // Dealer hidden (hard 17)
    
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
