#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QString>

class MainMenu;
class GameWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleStartNewGame(const QString& playerName);
    void handleContinueGame();
    void handleQuitRequested();
    void returnToMainMenu();

private:
    void setupWindow();

    QStackedWidget* m_stackedWidget;
    MainMenu* m_mainMenu;
    GameWindow* m_gameWindow;
};

#endif