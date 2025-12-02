#include "MainWindow.h"
#include "MainMenu.h"
#include "GameWindow.h"
#include "raylib.h"

MainWindow::MainWindow()
    : m_currentState(WindowState::MainMenu)
    , m_shouldClose(false)
{
    init();
}

MainWindow::~MainWindow() {
    cleanup();
}

void MainWindow::init() {
    // Initialize window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Blackjack - NeoSpace Casino");
    SetTargetFPS(60);
    
    // Create main menu
    m_mainMenu = std::make_unique<MainMenu>(this);
}

void MainWindow::run() {
    while (!WindowShouldClose() && !m_shouldClose) {
        update();
        draw();
    }
}

void MainWindow::update() {
    switch (m_currentState) {
        case WindowState::MainMenu:
            if (m_mainMenu) {
                m_mainMenu->update();
            }
            break;
        case WindowState::Game:
            if (m_gameWindow) {
                m_gameWindow->update();
            }
            break;
    }
}

void MainWindow::draw() {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){10, 15, 30, 255}); // #0a0f1e
    
    switch (m_currentState) {
        case WindowState::MainMenu:
            if (m_mainMenu) {
                m_mainMenu->draw();
            }
            break;
        case WindowState::Game:
            if (m_gameWindow) {
                m_gameWindow->draw();
            }
            break;
    }
    
    EndDrawing();
}

void MainWindow::handleStartNewGame(const std::string& playerName) {
    // Clean up existing game window if any
    if (m_gameWindow) {
        m_gameWindow.reset();
    }
    
    // Create new game window
    m_gameWindow = std::make_unique<GameWindow>(playerName, this);
    m_currentState = WindowState::Game;
}

void MainWindow::returnToMainMenu() {
    m_currentState = WindowState::MainMenu;
    // Keep game window alive in case player wants to continue
}

void MainWindow::handleQuit() {
    m_shouldClose = true;
}

void MainWindow::cleanup() {
    m_gameWindow.reset();
    m_mainMenu.reset();
    CloseWindow();
}
