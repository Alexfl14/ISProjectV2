#include "MainMenu.h"
#include "MainWindow.h"
#include "raylib.h"
#include <algorithm>

MainMenu::MainMenu(MainWindow* mainWindow)
    : m_mainWindow(mainWindow)
    , m_playerName("")
    , m_nameInputActive(false)
{
}

MainMenu::~MainMenu() {
}

void MainMenu::update() {
    handleInput();
}

void MainMenu::draw() {
    drawBackground();
    drawTitle();
    drawNameInput();
    drawButtons();
}

void MainMenu::drawBackground() {
    // Background is already cleared by MainWindow
}

void MainMenu::drawTitle() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Main title "NEOSPACE"
    const char* title = "NEOSPACE";
    int titleFontSize = 72;
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 6,
             titleFontSize, CLITERAL(Color){0, 255, 255, 255}); // Cyan
    
    // Subtitle "BLACKJACK"
    const char* subtitle = "BLACKJACK";
    int subtitleFontSize = 48;
    int subtitleWidth = MeasureText(subtitle, subtitleFontSize);
    DrawText(subtitle, (screenWidth - subtitleWidth) / 2, screenHeight / 6 + 90,
             subtitleFontSize, CLITERAL(Color){255, 215, 0, 255}); // Gold
}

void MainMenu::drawNameInput() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    int inputX = (screenWidth - INPUT_WIDTH) / 2;
    int inputY = screenHeight / 2 - 50;
    
    // Prompt text
    const char* prompt = "Enter your name:";
    int promptFontSize = 18;
    int promptWidth = MeasureText(prompt, promptFontSize);
    DrawText(prompt, (screenWidth - promptWidth) / 2, inputY - 40,
             promptFontSize, WHITE);
    
    // Input box
    Rectangle inputBox = {(float)inputX, (float)inputY, (float)INPUT_WIDTH, (float)INPUT_HEIGHT};
    
    Color borderColor = m_nameInputActive ? CLITERAL(Color){255, 215, 0, 255} : CLITERAL(Color){0, 255, 255, 255};
    DrawRectangleRec(inputBox, CLITERAL(Color){17, 24, 44, 255}); // #11182C
    DrawRectangleLinesEx(inputBox, 2, borderColor);
    
    // Check if clicked
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        m_nameInputActive = CheckCollisionPointRec(mousePos, inputBox);
    }
    
    // Handle text input
    if (m_nameInputActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && m_playerName.length() < 20) {
                m_playerName += (char)key;
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE) && !m_playerName.empty()) {
            m_playerName.pop_back();
        }
    }
    
    // Draw text or placeholder
    const char* displayText = m_playerName.empty() ? "Player Name" : m_playerName.c_str();
    Color textColor = m_playerName.empty() ? CLITERAL(Color){85, 85, 102, 255} : CLITERAL(Color){0, 255, 255, 255};
    
    int textFontSize = 20;
    int textWidth = MeasureText(displayText, textFontSize);
    DrawText(displayText, inputX + (INPUT_WIDTH - textWidth) / 2, 
             inputY + (INPUT_HEIGHT - textFontSize) / 2, textFontSize, textColor);
}

void MainMenu::drawButtons() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    int buttonX = (screenWidth - BUTTON_WIDTH) / 2;
    int newGameY = screenHeight / 2 + 80;
    int quitY = newGameY + BUTTON_HEIGHT + 20;
    
    // NEW GAME button
    Rectangle newGameBtn = {(float)buttonX, (float)newGameY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT};
    Vector2 mousePos = GetMousePosition();
    bool newGameHover = CheckCollisionPointRec(mousePos, newGameBtn);
    
    Color newGameBg = newGameHover ? CLITERAL(Color){26, 39, 68, 255} : CLITERAL(Color){17, 24, 44, 255};
    Color newGameBorder = newGameHover ? CLITERAL(Color){255, 215, 0, 255} : CLITERAL(Color){0, 255, 255, 255};
    Color newGameTextColor = newGameHover ? CLITERAL(Color){255, 215, 0, 255} : CLITERAL(Color){0, 255, 255, 255};
    
    DrawRectangleRounded(newGameBtn, 0.2f, 8, newGameBg);
    DrawRectangleRoundedLines(newGameBtn, 0.2f, 8, newGameBorder);
    
    const char* newGameTextStr = "NEW GAME";
    int newGameTextWidth = MeasureText(newGameTextStr, 20);
    DrawText(newGameTextStr, buttonX + (BUTTON_WIDTH - newGameTextWidth) / 2,
             newGameY + (BUTTON_HEIGHT - 20) / 2, 20, newGameTextColor);
    
    if (newGameHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (m_playerName.empty()) {
            // Show error - player name required (simple version)
        } else {
            m_mainWindow->handleStartNewGame(m_playerName);
        }
    }
    
    // QUIT button
    Rectangle quitBtn = {(float)buttonX, (float)quitY, (float)BUTTON_WIDTH, (float)BUTTON_HEIGHT};
    bool quitHover = CheckCollisionPointRec(mousePos, quitBtn);
    
    Color quitBg = quitHover ? CLITERAL(Color){42, 26, 26, 255} : CLITERAL(Color){26, 10, 10, 255};
    Color quitBorder = quitHover ? CLITERAL(Color){255, 102, 102, 255} : CLITERAL(Color){255, 68, 68, 255};
    Color quitTextColor = quitHover ? CLITERAL(Color){255, 102, 102, 255} : CLITERAL(Color){255, 68, 68, 255};
    
    DrawRectangleRounded(quitBtn, 0.2f, 8, quitBg);
    DrawRectangleRoundedLines(quitBtn, 0.2f, 8,quitBorder);
    
    const char* quitText = "QUIT";
    int quitTextWidth = MeasureText(quitText, 20);
    DrawText(quitText, buttonX + (BUTTON_WIDTH - quitTextWidth) / 2,
             quitY + (BUTTON_HEIGHT - 20) / 2, 20, quitTextColor);
    
    if (quitHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        m_mainWindow->handleQuit();
    }
    
    // Footer
    const char* footer = "© 2024 NeoSpace Casino - Play Responsibly";
    int footerFontSize = 12;
    int footerWidth = MeasureText(footer, footerFontSize);
    DrawText(footer, (screenWidth - footerWidth) / 2, screenHeight - 40,
             footerFontSize, CLITERAL(Color){85, 85, 102, 255});
}

void MainMenu::handleInput() {
    // Input handling is done in draw functions for immediate visual feedback
}
