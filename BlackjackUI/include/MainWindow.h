#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <memory>
#include "enums/EWindowState.h"
class MainMenu;
class GameWindow;



class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    void run();
    
    void handleStartNewGame(const std::string& playerName);
    void returnToMainMenu();
    void handleQuit();

private:
    void init();
    void update();
    void draw();
    void cleanup();

    EWindowState m_currentState;
    std::unique_ptr<MainMenu> m_mainMenu;
    std::unique_ptr<GameWindow> m_gameWindow;
    bool m_shouldClose;
};

#endif // MAINWINDOW_H
