#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Game.h"
#include "interfaces/IObserver.h"
#include "interfaces/ICard.h"
#include "raylib.h"
#include <string>
#include "enums/EGamePhase.h"
class MainWindow;

class GameWindow : public IObserver {
public:
    explicit GameWindow(const std::string& playerName, MainWindow* mainWindow);
    ~GameWindow() override;

    void update();
    void draw();

    // IObserver implementation
    void onNotify(const std::string& eventName) override;

private:
    // Game phases
    void showBettingPhase();
    void showGameplayPhase();
    void showResultPhase();

    // Input handlers
    void handleBettingInput();
    void handleGameplayInput();
    void handleResultInput();

    // Drawing functions
    void drawBackground();
    void drawTableFelt();
    void drawPlayerInfo();
    void drawDealerCards();
    void drawPlayerCards();
    void drawBettingControls();
    void drawGameControls();
    void drawResultOverlay();
    void drawCard(const ICard* card, float x, float y, bool hidden = false);
    void drawButton(const char* text, float x, float y, float width, float height, bool enabled, bool* clicked);
    
    // UI Updates
    void updateUI();
    void updateBalanceDisplay();
    void checkAvailableActions();

    // Helper functions
    std::string getCardValueString(const ICard* card) const;
    std::string getSuitSymbol(const ICard* card) const;
    Color getSuitColor(const ICard* card) const;

    MainWindow* m_mainWindow;
    Game m_game;
    std::string m_playerName;
    int m_currentBetAmount;
    

    EGamePhase m_currentPhase;

    // UI state
    bool m_hitEnabled;
    bool m_standEnabled;
    bool m_doubleEnabled;
    bool m_splitEnabled;
    bool m_insuranceEnabled;
    
    std::string m_resultMessage;
    
    // UI Constants
    static constexpr int CARD_WIDTH = 80;
    static constexpr int CARD_HEIGHT = 112;
    static constexpr int CARD_SPACING = 20;
    static constexpr int BUTTON_WIDTH = 120;
    static constexpr int BUTTON_HEIGHT = 50;
};

#endif // GAMEWINDOW_H
