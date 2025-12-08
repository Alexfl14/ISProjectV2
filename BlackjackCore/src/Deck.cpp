#include "Deck.h"

Deck::Deck(int numDecks, bool enableAutoReshuffle)
    : numberOfDecks(numDecks > 0 && numDecks <= 8 ? numDecks : 1),
      cutCardPosition(0),
      cardsDrawnSinceLastShuffle(0),
      autoReshuffle(enableAutoReshuffle),
      rng(std::random_device{}())  // Initialize with hardware entropy
{
    initializeCards();
    reseedRNG();  // Additional reseeding with time-based entropy
    shuffle();
    cutCardPosition = generateCutCardPosition();
}

void Deck::initializeCards() {
    cards.clear();
    cards.reserve(52 * numberOfDecks);

    // Create multiple decks (1-8 decks, like real casinos)
    for (int deck = 0; deck < numberOfDecks; ++deck) {
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
}

void Deck::reseedRNG() {
    // Mix multiple entropy sources for better unpredictability
    std::random_device rd;
    auto time_seed = static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    // Combine hardware entropy with high-resolution time
    std::seed_seq seed{rd(), rd(), time_seed, rd()};
    rng.seed(seed);
}

int Deck::generateCutCardPosition() {
    // Casino cut card is typically placed 60-80% through the shoe
    // This adds unpredictability to when reshuffles occur
    std::uniform_int_distribution<int> dist(
        static_cast<int>(cards.size() * 0.6),
        static_cast<int>(cards.size() * 0.8)
    );
    return dist(rng);
}

void Deck::shuffle() {
    // Reseed RNG before each shuffle for maximum unpredictability
    reseedRNG();

    // Use Fisher-Yates shuffle with our non-static RNG
    std::shuffle(cards.begin(), cards.end(), rng);

    // Reset tracking variables
    cardsDrawnSinceLastShuffle = 0;
    cutCardPosition = generateCutCardPosition();
}

std::unique_ptr<ICard> Deck::draw() {
    // Check if we need to reshuffle (hit cut card position)
    if (autoReshuffle && cardsDrawnSinceLastShuffle >= cutCardPosition && !cards.empty()) {
        // Reshuffle all cards back into the deck
        initializeCards();
        shuffle();
    }

    if (cards.empty()) return nullptr;

    auto card = std::move(cards.back());
    cards.pop_back();
    cardsDrawnSinceLastShuffle++;

    return card;
}

int Deck::size() const {
    return static_cast<int>(cards.size());
}

void Deck::setNumberOfDecks(int numDecks) {
    if (numDecks > 0 && numDecks <= 8) {
        numberOfDecks = numDecks;
        initializeCards();
        shuffle();
    }
}

int Deck::getNumberOfDecks() const {
    return numberOfDecks;
}

void Deck::setAutoReshuffle(bool enable) {
    autoReshuffle = enable;
}

bool Deck::getAutoReshuffle() const {
    return autoReshuffle;
}

int Deck::getCutCardPosition() const {
    return cutCardPosition;
}

int Deck::getCardsDrawnSinceLastShuffle() const {
    return cardsDrawnSinceLastShuffle;
}
