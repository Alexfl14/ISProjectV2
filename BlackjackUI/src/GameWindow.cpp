#include "GameWindow.h"
#include "MainWindow.h"
#include "raylib.h"
#include <sstream>
#include <iomanip>

GameWindow::GameWindow(const std::string& playerName, MainWindow* mainWindow)
    : m_mainWindow(mainWindow)
    , m_game(1000)  // Starting balance of 1000
    , m_playerName(playerName)
    , m_currentBetAmount(0)
    , m_currentPhase(GamePhase::Betting)
    , m_hitEnabled(false)
    , m_standEnabled(false)
    , m_doubleEnabled(false)
    , m_splitEnabled(false)
    , m_insuranceEnabled(false)
    , m_resultMessage("")
{
    // Register as observer
    m_game.addObserver(this);
    m_game.startGame();
}

GameWindow::~GameWindow() {
    m_game.removeObserver(this);
}

void GameWindow::update() {
    switch (m_currentPhase) {
        case GamePhase::Betting:
            handleBettingInput();
            break;
        case GamePhase::Playing:
            handleGameplayInput();
            break;
        case GamePhase::Result:
            handleResultInput();
            break;
    }
}

void GameWindow::draw() {
    drawBackground();
    drawTableFelt();
    drawPlayerInfo();
    drawDealerCards();
    drawPlayerCards();
    
    switch (m_currentPhase) {
        case GamePhase::Betting:
            drawBettingControls();
            break;
        case GamePhase::Playing:
            drawGameControls();
            break;
        case GamePhase::Result:
            drawResultOverlay();
            drawGameControls();  // Show new round button
            break;
    }
}

void GameWindow::onNotify(const std::string& eventName) {
    // Observer pattern - update UI based on game events
    if (eventName == "StartGame" || eventName == "Reset") {
        updateUI();
    } else if (eventName == "PlayerCardMain" || eventName == "PlayerCardSplit" || 
               eventName == "DealerCardVisible" || eventName == "DealerCardHidden") {
        updateUI();
    } else if (eventName == "RoundFinished") {
        updateUI();
        m_currentPhase = GamePhase::Result;
        
        // Determine result message
        GameState state = m_game.getState();
        if (state.resultMain == EGameResult::PlayerWon) {
            m_resultMessage = "YOU WIN!";
        } else if (state.resultMain == EGameResult::DealerWon) {
            m_resultMessage = "DEALER WINS";
        } else if (state.resultMain == EGameResult::Push) {
            m_resultMessage = "PUSH";
        }
    } else if (eventName == "BetChanged") {
        updateUI();
    } else if (eventName == "SplitPerformed" || eventName == "SwitchToSplitHand") {
        updateUI();
    } else if (eventName == "InsuranceTaken") {
        updateUI();
    }
}

void GameWindow::handleBettingInput() {
    // Betting input handled in drawBettingControls
}

void GameWindow::handleGameplayInput() {
    // Gameplay input handled in drawGameControls
}

void GameWindow::handleResultInput() {
    // Result input handled in drawGameControls (new round button)
}

void GameWindow::drawBackground() {
    // Background already cleared by MainWindow
}

void GameWindow::drawTableFelt() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Draw green felt table
    DrawRectangle(0, screenHeight / 4, screenWidth, screenHeight * 3 / 4, CLITERAL(Color){22, 108, 42, 255});
    
    // Draw decorative border
    DrawRectangleLinesEx(Rectangle{0, (float)screenHeight / 4, (float)screenWidth, (float)screenHeight * 3 / 4}, 
                         3, CLITERAL(Color){139, 69, 19, 255});
}

void GameWindow::drawPlayerInfo() {
    int screenWidth = GetScreenWidth();
    
    // Player name
    std::string nameText = "Player: " + m_playerName;
    DrawText(nameText.c_str(), 20, 20, 24, CLITERAL(Color){255, 215, 0, 255});
    
    // Balance
    std::ostringstream balanceStream;
    balanceStream << "Balance: $" << m_game.getPlayer().getBalance();
    DrawText(balanceStream.str().c_str(), 20, 50, 20, CLITERAL(Color){0, 255, 0, 255});
    
    // Current bet
    std::ostringstream betStream;
    betStream << "Bet: $" << m_game.getPlayer().getCurrentBet();
    DrawText(betStream.str().c_str(), 20, 75, 20, CLITERAL(Color){255, 215, 0, 255});
    
    // Insurance bet if any
    if (m_game.getPlayer().getInsuranceBet() > 0) {
        std::ostringstream insuranceStream;
        insuranceStream << "Insurance: $" << m_game.getPlayer().getInsuranceBet();
        DrawText(insuranceStream.str().c_str(), 20, 100, 20, CLITERAL(Color){255, 165, 0, 255});
    }
}

void GameWindow::drawDealerCards() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Dealer title
    const char* dealerTitle = "DEALER";
    int titleWidth = MeasureText(dealerTitle, 24);
    DrawText(dealerTitle, (screenWidth - titleWidth) / 2, screenHeight / 4 + 20, 24, WHITE);
    
    // Dealer total
    GameState state = m_game.getState();
    if (!state.dealerHidden || state.gameOver) {
        std::ostringstream totalStream;
        totalStream << "Total: " << state.dealerTotal;
        int totalWidth = MeasureText(totalStream.str().c_str(), 20);
        DrawText(totalStream.str().c_str(), (screenWidth - totalWidth) / 2, 
                 screenHeight / 4 + 50, 20, CLITERAL(Color){200, 200, 200, 255});
    }
    
    // Draw dealer cards
    const IHand& dealerHand = m_game.getDealerHand();
    int cardCount = dealerHand.getCardCount();
    int totalWidth = cardCount * (CARD_WIDTH + CARD_SPACING) - CARD_SPACING;
    int startX = (screenWidth - totalWidth) / 2;
    int cardY = screenHeight / 4 + 80;
    
    for (size_t i = 0; i < cardCount; ++i) {
        const ICard* card = dealerHand.getCard(i);
        bool hidden = (i == 1 && state.dealerHidden && !state.gameOver);
        drawCard(card, startX + i * (CARD_WIDTH + CARD_SPACING), cardY, hidden);
    }
}

void GameWindow::drawPlayerCards() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Player title
    const char* playerTitle = "PLAYER";
    int titleWidth = MeasureText(playerTitle, 24);
    DrawText(playerTitle, (screenWidth - titleWidth) / 2, screenHeight / 2 + 20, 24, WHITE);
    
    // Player total
    GameState state = m_game.getState();
    std::ostringstream totalStream;
    totalStream << "Total: " << state.mainTotal;
    int totalWidth = MeasureText(totalStream.str().c_str(), 20);
    DrawText(totalStream.str().c_str(), (screenWidth - totalWidth) / 2, 
             screenHeight / 2 + 50, 20, CLITERAL(Color){0, 255, 255, 255});
    
    // Draw main hand cards
    const IHand& playerHand = m_game.getPlayerHand();
    int cardCount = playerHand.getCardCount();
    int totalCardsWidth = cardCount * (CARD_WIDTH + CARD_SPACING) - CARD_SPACING;
    int startX = (screenWidth - totalCardsWidth) / 2;
    int cardY = screenHeight / 2 + 80;
    
    for (size_t i = 0; i < cardCount; ++i) {
        const ICard* card = playerHand.getCard(i);
        drawCard(card, (float)(startX + i * (CARD_WIDTH + CARD_SPACING)), (float)cardY, false);
    }
    
    // Draw split hand if exists
    if (state.splitActive && m_game.getPlayer().hasSplitHand()) {
        const IHand& splitHand = m_game.getPlayer().getSplitHand();
        int splitCardCount = splitHand.getCardCount();
        
        DrawText("SPLIT HAND", screenWidth / 2 + 150, screenHeight / 2 + 20, 20, CLITERAL(Color){255, 215, 0, 255});
        
        std::ostringstream splitTotalStream;
        splitTotalStream << "Total: " << state.splitTotal;
        DrawText(splitTotalStream.str().c_str(), screenWidth / 2 + 150, 
                 screenHeight / 2 + 45, 18, CLITERAL(Color){0, 255, 255, 255});
        
        for (size_t i = 0; i < splitCardCount; ++i) {
            const ICard* card = splitHand.getCard(i);
            drawCard(card, (float)(screenWidth / 2 + 150 + i * (CARD_WIDTH + CARD_SPACING)), 
                    (float)(screenHeight / 2 + 80), false);
        }
    }
}

void GameWindow::drawCard(const ICard* card, float x, float y, bool hidden) {
    Rectangle cardRect = {x, y, CARD_WIDTH, CARD_HEIGHT};
    
    if (hidden) {
        // Draw card back
        DrawRectangleRounded(cardRect, 0.1f, 8, CLITERAL(Color){0, 0, 139, 255});
        DrawRectangleRoundedLines(cardRect, 0.1f, 8, WHITE);
        
        // Draw pattern
        for (int i = 0; i < 3; ++i) {
            DrawCircle((int)(x + CARD_WIDTH / 2), (int)(y + 30 + i * 25), 8, CLITERAL(Color){255, 255, 255, 100});
        }
    } else {
        // Draw card face
        DrawRectangleRounded(cardRect, 0.1f, 8, WHITE);
        DrawRectangleRoundedLines(cardRect, 0.1f, 8, BLACK);
        
        Color suitColor = getSuitColor(card);
        std::string valueStr = getCardValueString(card);
        std::string suitStr = getSuitSymbol(card);
        
        // Draw value in top-left
        DrawText(valueStr.c_str(), (int)(x + 8), (int)(y + 8), 20, suitColor);
        
        // Draw suit symbol in center
        int suitFontSize = 32;
        int suitWidth = MeasureText(suitStr.c_str(), suitFontSize);
        DrawText(suitStr.c_str(), (int)(x + (CARD_WIDTH - suitWidth) / 2), 
                 (int)(y + (CARD_HEIGHT - suitFontSize) / 2), suitFontSize, suitColor);
        
        // Draw value in bottom-right (upside down effect)
        DrawText(valueStr.c_str(), (int)(x + CARD_WIDTH - 25), (int)(y + CARD_HEIGHT - 30), 20, suitColor);
    }
}

void GameWindow::drawBettingControls() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Betting title
    const char* title = "PLACE YOUR BET";
    int titleWidth = MeasureText(title, 28);
    DrawText(title, (screenWidth - titleWidth) / 2, screenHeight - 250, 28, CLITERAL(Color){255, 215, 0, 255});
    
    // Current bet amount
    std::ostringstream betStream;
    betStream << "Bet Amount: $" << m_currentBetAmount;
    int betWidth = MeasureText(betStream.str().c_str(), 24);
    DrawText(betStream.str().c_str(), (screenWidth - betWidth) / 2, 
             screenHeight - 210, 24, CLITERAL(Color){0, 255, 255, 255});
    
    // Betting buttons
    int buttonY = screenHeight - 170;
    int buttonSpacing = 10;
    int totalButtonsWidth = 5 * BUTTON_WIDTH + 4 * buttonSpacing;
    int startX = (screenWidth - totalButtonsWidth) / 2;
    
    int betAmounts[] = {25, 50, 100, 250, 500};
    const char* betLabels[] = {"$25", "$50", "$100", "$250", "$500"};
    
    for (int i = 0; i < 5; ++i) {
        bool clicked = false;
        float btnX = (float)(startX + i * (BUTTON_WIDTH + buttonSpacing));
        drawButton(betLabels[i], btnX, (float)buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, true, &clicked);
        
        if (clicked && m_game.requestBet(betAmounts[i])) {
            m_currentBetAmount += betAmounts[i];
        }
    }
    
    // Clear and Deal buttons
    buttonY = screenHeight - 100;
    int actionButtonWidth = 150;
    int actionStartX = (screenWidth - (actionButtonWidth * 2 + buttonSpacing)) / 2;
    
    bool clearClicked = false;
    drawButton("CLEAR BET", (float)actionStartX, (float)buttonY, (float)actionButtonWidth, BUTTON_HEIGHT, 
               m_currentBetAmount > 0, &clearClicked);
    if (clearClicked && m_game.requestClearBet()) {
        m_currentBetAmount = 0;
    }
    
    bool dealClicked = false;
    drawButton("DEAL", (float)(actionStartX + actionButtonWidth + buttonSpacing), (float)buttonY, 
               (float)actionButtonWidth, BUTTON_HEIGHT, m_currentBetAmount > 0, &dealClicked);
    if (dealClicked && m_currentBetAmount > 0) {
        // Start the game round - this will deal cards
        m_game.startGame();
        m_currentPhase = GamePhase::Playing;
        checkAvailableActions();
    }
}

void GameWindow::drawGameControls() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    GameState state = m_game.getState();
    
    if (m_currentPhase == GamePhase::Result) {
        // Show New Round button
        bool newRoundClicked = false;
        drawButton("NEW ROUND", (float)((screenWidth - 200) / 2), (float)(screenHeight - 80), 
                   200, BUTTON_HEIGHT, true, &newRoundClicked);
        if (newRoundClicked) {
            m_game.reset();
            m_currentBetAmount = 0;
            m_currentPhase = GamePhase::Betting;
            m_resultMessage = "";
        }
        return;
    }
    
    // Game action buttons
    int buttonY = screenHeight - 140;
    int buttonSpacing = 15;
    int numButtons = 5;
    int totalButtonsWidth = numButtons * BUTTON_WIDTH + (numButtons - 1) * buttonSpacing;
    int startX = (screenWidth - totalButtonsWidth) / 2;
    
    bool hitClicked = false;
    drawButton("HIT", (float)startX, (float)buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, m_hitEnabled, &hitClicked);
    if (hitClicked) {
        m_game.hit();
        checkAvailableActions();
    }
    
    bool standClicked = false;
    drawButton("STAND", (float)(startX + BUTTON_WIDTH + buttonSpacing), (float)buttonY, 
               BUTTON_WIDTH, BUTTON_HEIGHT, m_standEnabled, &standClicked);
    if (standClicked) {
        m_game.stand();
    }
    
    bool doubleClicked = false;
    drawButton("DOUBLE", (float)(startX + 2 * (BUTTON_WIDTH + buttonSpacing)), (float)buttonY, 
               BUTTON_WIDTH, BUTTON_HEIGHT, m_doubleEnabled, &doubleClicked);
    if (doubleClicked && m_game.requestDoubleDown()) {
        m_game.hit();
        m_game.stand();
    }
    
    bool splitClicked = false;
    drawButton("SPLIT", (float)(startX + 3 * (BUTTON_WIDTH + buttonSpacing)), (float)buttonY, 
               BUTTON_WIDTH, BUTTON_HEIGHT, m_splitEnabled, &splitClicked);
    if (splitClicked && m_game.requestSplit()) {
        checkAvailableActions();
    }
    
    bool insuranceClicked = false;
    drawButton("INSURANCE", (float)(startX + 4 * (BUTTON_WIDTH + buttonSpacing)), (float)buttonY, 
               BUTTON_WIDTH, BUTTON_HEIGHT, m_insuranceEnabled, &insuranceClicked);
    if (insuranceClicked) {
        m_game.requestInsurance();
        checkAvailableActions();
    }
    
    // Menu button
    bool menuClicked = false;
    drawButton("MENU", 20, (float)(screenHeight - 80), 100, 50, true, &menuClicked);
    if (menuClicked) {
        m_mainWindow->returnToMainMenu();
    }
}

void GameWindow::drawResultOverlay() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, CLITERAL(Color){0, 0, 0, 180});
    
    // Result message
    int fontSize = 64;
    int textWidth = MeasureText(m_resultMessage.c_str(), fontSize);
    Color textColor = CLITERAL(Color){255, 215, 0, 255};
    
    GameState state = m_game.getState();
    if (state.resultMain == EGameResult::PlayerWon) {
        textColor = CLITERAL(Color){0, 255, 0, 255};  // Green for win
    } else if (state.resultMain == EGameResult::DealerWon) {
        textColor = CLITERAL(Color){255, 68, 68, 255};  // Red for loss
    }
    
    DrawText(m_resultMessage.c_str(), (screenWidth - textWidth) / 2, 
             screenHeight / 2 - 50, fontSize, textColor);
}

void GameWindow::drawButton(const char* text, float x, float y, float width, float height, 
                           bool enabled, bool* clicked) {
    Rectangle btnRect = {x, y, width, height};
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, btnRect) && enabled;
    
    Color bgColor;
    Color borderColor;
    Color textColor;
    
    if (!enabled) {
        bgColor = CLITERAL(Color){50, 50, 50, 255};
        borderColor = CLITERAL(Color){100, 100, 100, 255};
        textColor = CLITERAL(Color){150, 150, 150, 255};
    } else if (hover) {
        bgColor = CLITERAL(Color){26, 39, 68, 255};
        borderColor = CLITERAL(Color){255, 215, 0, 255};
        textColor = CLITERAL(Color){255, 215, 0, 255};
    } else {
        bgColor = CLITERAL(Color){17, 24, 44, 255};
        borderColor = CLITERAL(Color){0, 255, 255, 255};
        textColor = CLITERAL(Color){0, 255, 255, 255};
    }
    
    DrawRectangleRounded(btnRect, 0.2f, 8, bgColor);
    DrawRectangleRoundedLines(btnRect, 0.2f, 8, borderColor);
    
    int textWidth = MeasureText(text, 18);
    DrawText(text, (int)(x + (width - textWidth) / 2), (int)(y + (height - 18) / 2), 18, textColor);
    
    if (clicked) {
        *clicked = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
}

void GameWindow::updateUI() {
    checkAvailableActions();
}

void GameWindow::updateBalanceDisplay() {
    // Balance is drawn each frame in drawPlayerInfo
}

void GameWindow::checkAvailableActions() {
    GameState state = m_game.getState();
    
    m_hitEnabled = state.roundStarted && !state.gameOver;
    m_standEnabled = state.roundStarted && !state.gameOver;
    m_doubleEnabled = state.doubleAllowed;
    m_splitEnabled = state.splitAllowed;
    m_insuranceEnabled = state.insuranceAllowed;
}

std::string GameWindow::getCardValueString(const ICard* card) const {
    ECardValue value = card->getValue();
    
    if (value == ECardValue::Ace) return "A";
    else if (value == ECardValue::Two) return "2";
    else if (value == ECardValue::Three) return "3";
    else if (value == ECardValue::Four) return "4";
    else if (value == ECardValue::Five) return "5";
    else if (value == ECardValue::Six) return "6";
    else if (value == ECardValue::Seven) return "7";
    else if (value == ECardValue::Eight) return "8";
    else if (value == ECardValue::Nine) return "9";
    else if (value == ECardValue::Ten) return "10";
    else if (value == ECardValue::Jack) return "J";
    else if (value == ECardValue::Queen) return "Q";
    else if (value == ECardValue::King) return "K";
    else return "?";
}

std::string GameWindow::getSuitSymbol(const ICard* card) const {
    switch (card->getSuit()) {
        case ESuit::Hearts: return "H";
        case ESuit::Diamonds: return "D";
        case ESuit::Clubs: return "C";
        case ESuit::Spades: return "S";
        default: return "?";
    }
}

Color GameWindow::getSuitColor(const ICard* card) const {
    ESuit suit = card->getSuit();
    if (suit == ESuit::Hearts || suit == ESuit::Diamonds) {
        return RED;
    }
    return BLACK;
}

void GameWindow::showBettingPhase() {
    m_currentPhase = GamePhase::Betting;
}

void GameWindow::showGameplayPhase() {
    m_currentPhase = GamePhase::Playing;
}

void GameWindow::showResultPhase() {
    m_currentPhase = GamePhase::Result;
}
