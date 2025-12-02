#include "MainMenu.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

MainMenu::MainMenu(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

MainMenu::~MainMenu()
{
}

void MainMenu::setupUI()
{
    setStyleSheet("background-color: #0a0f1e;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    mainLayout->addStretch(2);

    m_lblTitle = new QLabel("NEOSPACE", this);
    m_lblTitle->setAlignment(Qt::AlignCenter);
    m_lblTitle->setStyleSheet(
        "QLabel { color: #00FFFF; font-size: 72px; font-weight: bold; "
        "font-family: 'Segoe UI', Arial; background-color: transparent; letter-spacing: 15px; }"
    );
    mainLayout->addWidget(m_lblTitle);

    m_lblSubtitle = new QLabel("BLACKJACK", this);
    m_lblSubtitle->setAlignment(Qt::AlignCenter);
    m_lblSubtitle->setStyleSheet(
        "QLabel { color: #FFD700; font-size: 48px; font-weight: bold; "
        "font-family: 'Segoe UI', Arial; background-color: transparent; letter-spacing: 20px; }"
    );
    mainLayout->addWidget(m_lblSubtitle);

    mainLayout->addStretch(1);

    QWidget* inputContainer = new QWidget(this);
    inputContainer->setMaximumWidth(500);
    inputContainer->setStyleSheet("background-color: transparent;");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputContainer);
    inputLayout->setSpacing(15);

    m_lblNamePrompt = new QLabel("Enter your name:", this);
    m_lblNamePrompt->setAlignment(Qt::AlignCenter);
    m_lblNamePrompt->setStyleSheet(
        "QLabel { color: #FFFFFF; font-size: 18px; font-family: 'Segoe UI', Arial; background-color: transparent; }"
    );
    inputLayout->addWidget(m_lblNamePrompt);

    m_txtPlayerName = new QLineEdit(this);
    m_txtPlayerName->setPlaceholderText("Player Name");
    m_txtPlayerName->setAlignment(Qt::AlignCenter);
    m_txtPlayerName->setMinimumHeight(50);
    m_txtPlayerName->setStyleSheet(
        "QLineEdit { background-color: #11182C; color: #00FFFF; border: 2px solid #00FFFF; "
        "border-radius: 10px; padding: 10px 20px; font-size: 20px; font-family: 'Segoe UI', Arial; }"
        "QLineEdit:focus { border: 2px solid #FFD700; }"
        "QLineEdit::placeholder { color: #555566; }"
    );
    inputLayout->addWidget(m_txtPlayerName);

    QHBoxLayout* inputCenterLayout = new QHBoxLayout();
    inputCenterLayout->addStretch();
    inputCenterLayout->addWidget(inputContainer);
    inputCenterLayout->addStretch();
    mainLayout->addLayout(inputCenterLayout);

    mainLayout->addStretch(1);

    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet("background-color: transparent;");
    QVBoxLayout* buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(15);

    QString buttonStyle =
        "QPushButton { background-color: #11182C; color: #00FFFF; border: 2px solid #00FFFF; "
        "border-radius: 12px; padding: 15px 60px; font-size: 20px; font-weight: bold; "
        "font-family: 'Segoe UI', Arial; min-width: 250px; }"
        "QPushButton:hover { background-color: #1a2744; border-color: #FFD700; color: #FFD700; }"
        "QPushButton:pressed { background-color: #0a0f1a; }";

    m_btnNewGame = new QPushButton("NEW GAME", this);
    m_btnNewGame->setStyleSheet(buttonStyle);
    m_btnNewGame->setCursor(Qt::PointingHandCursor);
    connect(m_btnNewGame, &QPushButton::clicked, this, &MainMenu::onNewGameClicked);
    buttonLayout->addWidget(m_btnNewGame, 0, Qt::AlignCenter);

    QString quitButtonStyle =
        "QPushButton { background-color: #1a0a0a; color: #FF4444; border: 2px solid #FF4444; "
        "border-radius: 12px; padding: 15px 60px; font-size: 20px; font-weight: bold; "
        "font-family: 'Segoe UI', Arial; min-width: 250px; }"
        "QPushButton:hover { background-color: #2a1a1a; border-color: #FF6666; color: #FF6666; }"
        "QPushButton:pressed { background-color: #0a0505; }";

    m_btnQuit = new QPushButton("QUIT", this);
    m_btnQuit->setStyleSheet(quitButtonStyle);
    m_btnQuit->setCursor(Qt::PointingHandCursor);
    connect(m_btnQuit, &QPushButton::clicked, this, &MainMenu::onQuitClicked);
    buttonLayout->addWidget(m_btnQuit, 0, Qt::AlignCenter);

    QHBoxLayout* buttonCenterLayout = new QHBoxLayout();
    buttonCenterLayout->addStretch();
    buttonCenterLayout->addWidget(buttonContainer);
    buttonCenterLayout->addStretch();
    mainLayout->addLayout(buttonCenterLayout);

    mainLayout->addStretch(2);

    QLabel* lblFooter = new QLabel("© 2024 NeoSpace Casino - Play Responsibly", this);
    lblFooter->setAlignment(Qt::AlignCenter);
    lblFooter->setStyleSheet(
        "QLabel { color: #555566; font-size: 12px; font-family: 'Segoe UI', Arial; background-color: transparent; }"
    );
    mainLayout->addWidget(lblFooter);
}

void MainMenu::onNewGameClicked()
{
    QString name = m_txtPlayerName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Name Required");
        msgBox.setText("Please enter your name to start the game!");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #11182C; }"
            "QMessageBox QLabel { color: #00FFFF; font-size: 14px; }"
            "QPushButton { background-color: #11182C; color: #00FFFF; border: 2px solid #00FFFF; "
            "border-radius: 5px; padding: 8px 20px; font-size: 14px; }"
        );
        msgBox.exec();
        return;
    }
    emit startNewGame(name);
}

void MainMenu::onContinueClicked()
{
}

void MainMenu::onQuitClicked()
{
    emit quitRequested();
}