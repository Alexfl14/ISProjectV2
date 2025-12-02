#include "MainWindow.h"
#include "MainMenu.h"
#include "GameWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QScreen>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_mainMenu(nullptr)
    , m_gameWindow(nullptr)
{
    setupWindow();

    m_mainMenu = new MainMenu(this);
    m_stackedWidget->addWidget(m_mainMenu);
    setCentralWidget(m_stackedWidget);

    connect(m_mainMenu, &MainMenu::startNewGame, this, &MainWindow::handleStartNewGame);
    connect(m_mainMenu, &MainMenu::quitRequested, this, &MainWindow::handleQuitRequested);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupWindow()
{
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int width = screenGeometry.width() * 0.85;
    int height = screenGeometry.height() * 0.85;
    int x = (screenGeometry.width() - width) / 2;
    int y = (screenGeometry.height() - height) / 2;

    setGeometry(x, y, width, height);
    setMinimumSize(1024, 768);
    setWindowTitle("Blackjack - NeoSpace Casino");
    setStyleSheet("background-color: #0a0f1e;");
}

void MainWindow::handleStartNewGame(const QString& playerName)
{
    if (m_gameWindow) {
        m_stackedWidget->removeWidget(m_gameWindow);
        delete m_gameWindow;
        m_gameWindow = nullptr;
    }

    m_gameWindow = new GameWindow(playerName, this);
    connect(m_gameWindow, &GameWindow::returnToMenu, this, &MainWindow::returnToMainMenu);
    m_stackedWidget->addWidget(m_gameWindow);
    m_stackedWidget->setCurrentWidget(m_gameWindow);
}

void MainWindow::handleContinueGame()
{
    if (m_gameWindow) {
        m_stackedWidget->setCurrentWidget(m_gameWindow);
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("No Game");
        msgBox.setText("No game in progress. Please start a new game!");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #11182C; }"
            "QMessageBox QLabel { color: #00FFFF; font-size: 14px; }"
            "QPushButton { background-color: #11182C; color: #00FFFF; border: 2px solid #00FFFF; border-radius: 5px; padding: 8px 20px; }"
        );
        msgBox.exec();
    }
}

void MainWindow::handleQuitRequested()
{
    QApplication::quit();
}

void MainWindow::returnToMainMenu()
{
    m_stackedWidget->setCurrentWidget(m_mainMenu);
}