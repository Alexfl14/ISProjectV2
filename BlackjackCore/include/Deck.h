#pragma once

#include "interfaces/IDeck.h"
#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

class Deck : public IDeck {
private:
    std::vector<std::unique_ptr<ICard>> cards;
    int numberOfDecks;           // Support for multi-deck shoe (1-8 decks)
    int cutCardPosition;         // Position where reshuffle is triggered
    int cardsDrawnSinceLastShuffle; // Track card penetration
    bool autoReshuffle;          // Automatically reshuffle when hitting cut card
    std::mt19937 rng;            // Non-static RNG instance for better unpredictability

    void initializeCards();      // Helper to create cards for multiple decks
    void reseedRNG();            // Reseed RNG with better entropy mixing
    int generateCutCardPosition(); // Random penetration point (60-80% through deck)

public:
    Deck(int numDecks = 1, bool enableAutoReshuffle = true);
    void shuffle() override;
    std::unique_ptr<ICard> draw() override;
    int size() const override;

    // New methods for enhanced randomness control
    void setNumberOfDecks(int numDecks);
    int getNumberOfDecks() const;
    void setAutoReshuffle(bool enable);
    bool getAutoReshuffle() const;
    int getCutCardPosition() const;
    int getCardsDrawnSinceLastShuffle() const;
};
