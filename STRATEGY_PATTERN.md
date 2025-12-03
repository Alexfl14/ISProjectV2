# Strategy Design Pattern - Dealer Strategy Implementation

## Descriere

Acest proiect implementează **Strategy Design Pattern** pentru a gestiona comportamentul dealer-ului în jocul de Blackjack. Pattern-ul permite schimbarea dinamică a strategiei dealer-ului fără a modifica logica principală a jocului.

## Structura Implementării

### 1. Interface Strategy (`IDealerStrategy`)

```cpp
class IDealerStrategy {
public:
    virtual ~IDealerStrategy() = default;
    virtual bool shouldHit(const IHand& dealerHand) const = 0;
    virtual const char* getStrategyName() const = 0;
};
```

**Locație**: `BlackjackCore/include/interfaces/IDealerStrategy.h`

### 2. Concrete Strategies

#### StandardDealerStrategy

- **Regulă**: Dealer-ul trage cărți (hit) la 16 sau mai puțin, stă (stand) la 17 sau mai mult
- **Locație**:
  - Header: `BlackjackCore/include/StandardDealerStrategy.h`
  - Implementation: `BlackjackCore/src/StandardDealerStrategy.cpp`

```cpp
bool StandardDealerStrategy::shouldHit(const IHand& dealerHand) const {
    return dealerHand.getTotal() < 17;
}
```

#### HitOnSoft17Strategy

- **Regulă**: Dealer-ul trage cărți la soft 17 (Ace + 6), dar stă la hard 17
- **Soft 17**: Un total de 17 care include un As numărat ca 11 (ex: A-6)
- **Locație**:
  - Header: `BlackjackCore/include/HitOnSoft17Strategy.h`
  - Implementation: `BlackjackCore/src/HitOnSoft17Strategy.cpp`

```cpp
bool HitOnSoft17Strategy::shouldHit(const IHand& dealerHand) const {
    int total = dealerHand.getTotal();
    if (total < 17) return true;
    if (total > 17) return false;
    if (total == 17 && isSoft17(dealerHand)) return true;
    return false;
}
```

### 3. Context (`Game`)

Clasa `Game` utilizează strategia dealer-ului prin dependency injection:

```cpp
class Game : public IGame {
private:
    std::unique_ptr<IDealerStrategy> dealerStrategy;

public:
    void setDealerStrategy(std::unique_ptr<IDealerStrategy> strategy);
};
```

În metoda `dealerPlay()`:

```cpp
void Game::dealerPlay() {
    player.resolveInsurance(dealerBlackjack);

    // Folosește strategy pattern pentru a determina când dealer-ul trage
    while (dealerStrategy && dealerStrategy->shouldHit(dealerHand)) {
        if (auto c = deckPtr->draw()) {
            dealerHand.addCard(std::move(c));
            notify("DealerCardVisible");
        }
        else break;
    }
}
```

### 4. Factory Integration

`GameFactory` oferă metode pentru a crea jocuri cu strategii diferite:

```cpp
class GameFactory : public IGameFactory {
public:
    IGame* createGame() override;
    IGame* createGameWithStandardRules();
    IGame* createGameWithSoft17Rules();
};
```

## Utilizare

### Exemplu 1: Crearea unui joc cu reguli standard

```cpp
GameFactory factory;
IGame* game = factory.createGameWithStandardRules();
// Dealer-ul va sta la orice 17
```

### Exemplu 2: Crearea unui joc cu regula Hit on Soft 17

```cpp
GameFactory factory;
IGame* game = factory.createGameWithSoft17Rules();
// Dealer-ul va trage la soft 17, dar va sta la hard 17
```

### Exemplu 3: Schimbarea dinamică a strategiei

```cpp
Game game;
game.setDealerStrategy(std::make_unique<StandardDealerStrategy>());
// Joacă câteva runde...

// Schimbă strategia
game.setDealerStrategy(std::make_unique<HitOnSoft17Strategy>());
// Continuă cu noua strategie
```

## Avantaje ale Implementării

1. **Open/Closed Principle**: Poți adăuga strategii noi fără a modifica codul existent
2. **Flexibilitate**: Strategia poate fi schimbată la runtime
3. **Testabilitate**: Fiecare strategie poate fi testată independent
4. **Separarea responsabilităților**: Logica de decizie a dealer-ului este separată de logica jocului

## Extensibilitate

Pentru a adăuga o nouă strategie:

1. Creează o clasă nouă care implementează `IDealerStrategy`
2. Implementează metoda `shouldHit()` cu logica dorită
3. Adaugă o metodă în `GameFactory` dacă este necesar

### Exemplu de strategie nouă:

```cpp
// AggressiveDealerStrategy.h
class AggressiveDealerStrategy : public IDealerStrategy {
public:
    bool shouldHit(const IHand& dealerHand) const override {
        // Dealer mai agresiv: trage până la 18
        return dealerHand.getTotal() < 18;
    }

    const char* getStrategyName() const override {
        return "Aggressive (Stand on 18)";
    }
};
```

## Teste

Testele pentru Strategy pattern se găsesc în:

- `BlackjackTests/src/core_tests/DealerStrategyTests.cpp`

Teste incluse:

- ✅ StandardStrategy stă la 17
- ✅ StandardStrategy trage la 16
- ✅ HitOnSoft17Strategy trage la soft 17
- ✅ HitOnSoft17Strategy stă la hard 17
- ✅ Strategiile pot fi schimbate dinamic
- ✅ GameFactory creează jocuri cu strategii diferite

## Diagrama UML

```
┌─────────────────────────┐
│   IDealerStrategy       │
│   <<interface>>         │
├─────────────────────────┤
│ + shouldHit(): bool     │
│ + getStrategyName(): str│
└───────────┬─────────────┘
            │
            │ implements
     ┌──────┴───────┐
     │              │
┌────▼──────────┐  ┌▼──────────────────┐
│Standard       │  │HitOnSoft17        │
│DealerStrategy │  │Strategy           │
├───────────────┤  ├───────────────────┤
│+ shouldHit()  │  │+ shouldHit()      │
│               │  │+ isSoft17()       │
└───────────────┘  └───────────────────┘

┌─────────────────────────┐
│       Game              │
├─────────────────────────┤
│- dealerStrategy: ptr    │
├─────────────────────────┤
│+ setDealerStrategy()    │
│+ dealerPlay()           │
└─────────────────────────┘
        uses ▲
             │
             │
        IDealerStrategy
```

## Concluzie

Implementarea Strategy Pattern în acest proiect demonstrează:

- Utilizarea corectă a polimorfismului
- Separarea responsabilităților
- Cod extensibil și ușor de întreținut
- Testabilitate îmbunătățită
