#ifndef MAINMENU_H
#define MAINMENU_H

#include <string>

class MainWindow;

class MainMenu {
public:
    explicit MainMenu(MainWindow* mainWindow);
    ~MainMenu();

    void update();
    void draw();

private:
    void drawBackground();
    void drawTitle();
    void drawNameInput();
    void drawButtons();
    void handleInput();

    MainWindow* m_mainWindow;
    std::string m_playerName;
    bool m_nameInputActive;
    
    // UI Constants
    static constexpr int BUTTON_WIDTH = 250;
    static constexpr int BUTTON_HEIGHT = 60;
    static constexpr int INPUT_WIDTH = 400;
    static constexpr int INPUT_HEIGHT = 50;
};

#endif // MAINMENU_H
