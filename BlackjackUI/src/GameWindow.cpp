#include "GameWindow.h"
#include <QLayoutItem>
#include <QMessageBox>
#include <QScreen>
#include <QResizeEvent>
#include <QApplication>
#include <QSizePolicy>

GameWindow::GameWindow(const QString& playerName, QWidget* parent)
    : QWidget(parent)
    , m_game(1000)
    , m_playerName(playerName.isEmpty() ? "Player" : playerName)
    , m_currentBetAmount(0)
    , m_bettingPhase(true)
    , m_mainLayout(nullptr)
    , m_dealerCardsLayout(nullptr)
    , m_playerCardsLayout(nullptr)
    , m_playerSplitCardsLayout(nullptr)
    , m_splitHandContainer(nullptr)
    , m_lblSplitHandTitle(nullptr)
    , m_lblPlayerName(nullptr)
    , m_lblBalance(nullptr)
    , m_lblCurrentBet(nullptr)
    , m_lblInsuranceBet(nullptr)
    , m_lblDealerTitle(nullptr)
    , m_lblDealerTotal(nullptr)
    , m_lblPlayerTitle(nullptr)
    , m_lblPlayerTotal(nullptr)
    , m_btnHit(nullptr)
    , m_btnStand(nullptr)
    , m_btnDouble(nullptr)
    , m_btnSplit(nullptr)
    , m_btnInsurance(nullptr)
    , m_btnNewRound(nullptr)
    , m_bettingWidget(nullptr)
{
    setupUI();
    showBettingPhase();
    updateBalanceDisplay();
}

GameWindow::~GameWindow()
{
}

void GameWindow::setupUI()
{
    setMinimumSize(1200, 850);
    setWindowTitle("Blackjack - NeoSpace Casino");

    setupTableBackground();

    QHBoxLayout* mainHorizontalLayout = new QHBoxLayout(this);
    mainHorizontalLayout->setContentsMargins(15, 10, 15, 10);
    mainHorizontalLayout->setSpacing(15);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setSpacing(8);

    setupInfoLabels();

    m_mainLayout->addSpacing(10);

    QWidget* dealerArea = new QWidget(this);
    QVBoxLayout* dealerLayout = new QVBoxLayout(dealerArea);
    dealerLayout->setSpacing(8);

    m_lblDealerTitle = new QLabel("DEALER", this);
    m_lblDealerTitle->setAlignment(Qt::AlignCenter);
    m_lblDealerTitle->setStyleSheet(
        "QLabel { color: #00FFFF; font-size: 26px; font-weight: bold; "
        "background-color: #11182C; padding: 8px 22px; border-radius: 8px; }"
    );
    dealerLayout->addWidget(m_lblDealerTitle, 0, Qt::AlignCenter);

    m_lblDealerTotal = new QLabel("Dealer: ?", this);
    m_lblDealerTotal->setAlignment(Qt::AlignCenter);
    m_lblDealerTotal->setStyleSheet(
        "QLabel { color: #00FFFF; font-size: 16px; font-weight: bold; "
        "background-color: #11182C; padding: 6px 15px; border-radius: 6px; }"
    );
    dealerLayout->addWidget(m_lblDealerTotal, 0, Qt::AlignCenter);

    QWidget* dealerCardsWidget = new QWidget(dealerArea);
    dealerCardsWidget->setMinimumHeight(160);
    m_dealerCardsLayout = new QHBoxLayout(dealerCardsWidget);
    m_dealerCardsLayout->setSpacing(15);
    m_dealerCardsLayout->setContentsMargins(0, 8, 0, 8);
    dealerLayout->addWidget(dealerCardsWidget, 0, Qt::AlignCenter);

    m_mainLayout->addWidget(dealerArea);

    m_mainLayout->addStretch();

    QWidget* playerArea = new QWidget(this);
    QVBoxLayout* playerLayout = new QVBoxLayout(playerArea);
    playerLayout->setSpacing(8);
    playerLayout->setContentsMargins(0, 0, 0, 0);

    m_lblPlayerTotal = new QLabel("Your Hand: ?", this);
    m_lblPlayerTotal->setAlignment(Qt::AlignCenter);
    m_lblPlayerTotal->setStyleSheet(
        "QLabel { color: #00FFFF; font-size: 16px; font-weight: bold; "
        "background-color: #11182C; padding: 6px 15px; border-radius: 6px; }"
    );
    playerLayout->addWidget(m_lblPlayerTotal, 0, Qt::AlignCenter);

    QWidget* playerCardsWidget = new QWidget(playerArea);
    playerCardsWidget->setMinimumHeight(160);
    m_playerCardsLayout = new QHBoxLayout(playerCardsWidget);
    m_playerCardsLayout->setSpacing(15);
    m_playerCardsLayout->setContentsMargins(0, 8, 0, 8);
    playerLayout->addWidget(playerCardsWidget, 0, Qt::AlignCenter);

    m_splitHandContainer = new QWidget(playerArea);
    m_splitHandContainer->setVisible(false);
    m_splitHandContainer->setMinimumHeight(180);
    QVBoxLayout* splitContainerLayout = new QVBoxLayout(m_splitHandContainer);
    splitContainerLayout->setSpacing(6);
    splitContainerLayout->setContentsMargins(0, 8, 0, 8);

    m_lblSplitHandTitle = new QLabel("⭐ SPLIT HAND ⭐", m_splitHandContainer);
    m_lblSplitHandTitle->setAlignment(Qt::AlignCenter);
    m_lblSplitHandTitle->setStyleSheet(
        "QLabel { color: #FFD700; font-size: 14px; font-weight: bold; "
        "background-color: #11182C; padding: 4px 10px; border-radius: 5px; }"
    );
    splitContainerLayout->addWidget(m_lblSplitHandTitle, 0, Qt::AlignCenter);

    QWidget* splitCardsWidget = new QWidget(m_splitHandContainer);
    splitCardsWidget->setMinimumHeight(160);
    m_playerSplitCardsLayout = new QHBoxLayout(splitCardsWidget);
    m_playerSplitCardsLayout->setSpacing(15);
    m_playerSplitCardsLayout->setContentsMargins(0, 6, 0, 8);
    splitContainerLayout->addWidget(splitCardsWidget, 0, Qt::AlignCenter);

    playerLayout->addWidget(m_splitHandContainer, 0, Qt::AlignCenter);

    m_mainLayout->addWidget(playerArea);

    m_mainLayout->addSpacing(10);

    mainHorizontalLayout->addLayout(m_mainLayout);

    setupGameControls();
    mainHorizontalLayout->addWidget(createVerticalControlsPanel());

    setupBettingWidget();
}

QWidget* GameWindow::createVerticalControlsPanel()
{
    QWidget* controlsPanel = new QWidget(this);
    controlsPanel->setStyleSheet("background-color: rgba(17, 24, 44, 0.8); border-radius: 15px;");
    controlsPanel->setFixedWidth(180);

    QVBoxLayout* panelLayout = new QVBoxLayout(controlsPanel);
    panelLayout->setSpacing(12);
    panelLayout->setContentsMargins(15, 20, 15, 20);

    QLabel* titleLabel = new QLabel("ACTIONS", controlsPanel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { color: #FFD700; font-size: 18px; font-weight: bold; "
        "background-color: transparent; padding: 8px; }"
    );
    panelLayout->addWidget(titleLabel);

    panelLayout->addSpacing(10);

    QString buttonStyle =
        "QPushButton { background-color: #11182C; color: #00FFFF; border: 2px solid #00FFFF; "
        "border-radius: 8px; padding: 14px 10px; font-family: 'Segoe UI', Arial; "
        "font-size: 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1a2744; border-color: #FFD700; }"
        "QPushButton:pressed { background-color: #0a0f1a; }"
        "QPushButton:disabled { background-color: #2a2a3a; color: #555566; border-color: #444455; }";

    panelLayout->addWidget(m_btnHit);
    m_btnHit->setStyleSheet(buttonStyle);
    m_btnHit->setFixedHeight(55);

    panelLayout->addWidget(m_btnStand);
    m_btnStand->setStyleSheet(buttonStyle);
    m_btnStand->setFixedHeight(55);

    panelLayout->addWidget(m_btnDouble);
    m_btnDouble->setStyleSheet(buttonStyle);
    m_btnDouble->setFixedHeight(55);

    panelLayout->addWidget(m_btnSplit);
    m_btnSplit->setStyleSheet(buttonStyle);
    m_btnSplit->setFixedHeight(55);

    panelLayout->addWidget(m_btnInsurance);
    m_btnInsurance->setStyleSheet(buttonStyle);
    m_btnInsurance->setFixedHeight(55);

    panelLayout->addSpacing(15);

    panelLayout->addWidget(m_btnNewRound);
    m_btnNewRound->setStyleSheet(
        "QPushButton { background-color: #1a5c2e; color: #00FF00; border: 2px solid #00FF00; "
        "border-radius: 8px; padding: 14px 10px; font-family: 'Segoe UI', Arial; "
        "font-size: 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2a7c4e; border-color: #FFD700; }"
        "QPushButton:pressed { background-color: #0a3c1e; }"
    );
    m_btnNewRound->setFixedHeight(55);

    panelLayout->addStretch();

    return controlsPanel;
}

void GameWindow::setupTableBackground()
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#0d5c2e"));
    setPalette(pal);

    setStyleSheet(
        "QWidget { background-color: #0d5c2e; }"
        "QLabel { color: #FFFFFF; font-family: 'Segoe UI', Arial; background-color: transparent; }"
    );
}

void GameWindow::setupInfoLabels()
{
    QString labelStyle =
        "QLabel { color: #00FFFF; font-family: 'Segoe UI', Arial; font-size: 14px; font-weight: bold; "
        "padding: 5px 12px; background-color: #11182C; border: 2px solid #00FFFF; border-radius: 6px; }";

    QString playerNameStyle =
        "QLabel { color: #FFD700; font-family: 'Segoe UI', Arial; font-size: 16px; font-weight: bold; "
        "padding: 6px 18px; background-color: #11182C; border: 2px solid #FFD700; border-radius: 6px; }";

    QHBoxLayout* infoLayout = new QHBoxLayout();
    infoLayout->setSpacing(10);

    m_lblPlayerName = new QLabel(this);
    m_lblPlayerName->setStyleSheet(playerNameStyle);
    m_lblPlayerName->setFixedHeight(38);
    infoLayout->addWidget(m_lblPlayerName);

    m_lblBalance = new QLabel(this);
    m_lblBalance->setStyleSheet(labelStyle);
    m_lblBalance->setFixedHeight(35);
    infoLayout->addWidget(m_lblBalance);

    m_lblCurrentBet = new QLabel(this);
    m_lblCurrentBet->setStyleSheet(labelStyle);
    m_lblCurrentBet->setFixedHeight(35);
    infoLayout->addWidget(m_lblCurrentBet);

    m_lblInsuranceBet = new QLabel(this);
    m_lblInsuranceBet->setStyleSheet(labelStyle);
    m_lblInsuranceBet->setFixedHeight(35);
    m_lblInsuranceBet->hide();
    infoLayout->addWidget(m_lblInsuranceBet);

    infoLayout->addStretch();

    m_mainLayout->addLayout(infoLayout);
}

void GameWindow::setupGameControls()
{
    m_btnHit = new QPushButton("HIT", this);
    m_btnHit->setCursor(Qt::PointingHandCursor);
    connect(m_btnHit, &QPushButton::clicked, this, &GameWindow::onHitClicked);

    m_btnStand = new QPushButton("STAND", this);
    m_btnStand->setCursor(Qt::PointingHandCursor);
    connect(m_btnStand, &QPushButton::clicked, this, &GameWindow::onStandClicked);

    m_btnDouble = new QPushButton("DOUBLE", this);
    m_btnDouble->setCursor(Qt::PointingHandCursor);
    connect(m_btnDouble, &QPushButton::clicked, this, &GameWindow::onDoubleClicked);

    m_btnSplit = new QPushButton("SPLIT", this);
    m_btnSplit->setCursor(Qt::PointingHandCursor);
    connect(m_btnSplit, &QPushButton::clicked, this, &GameWindow::onSplitClicked);

    m_btnInsurance = new QPushButton("INSURANCE", this);
    m_btnInsurance->setCursor(Qt::PointingHandCursor);
    connect(m_btnInsurance, &QPushButton::clicked, this, &GameWindow::onInsuranceClicked);

    m_btnNewRound = new QPushButton("NEW ROUND", this);
    m_btnNewRound->setCursor(Qt::PointingHandCursor);
    connect(m_btnNewRound, &QPushButton::clicked, this, &GameWindow::onNewRoundClicked);
}

void GameWindow::setupBettingWidget()
{
    m_bettingWidget = new QWidget(this);
    m_bettingWidget->setStyleSheet("background-color: rgba(17, 24, 44, 0.95); border-radius: 20px;");

    QVBoxLayout* mainLayout = new QVBoxLayout(m_bettingWidget);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    QLabel* lblTitle = new QLabel("PLACE YOUR BET", m_bettingWidget);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet(
        "QLabel { color: #FFD700; font-size: 32px; font-weight: bold; background-color: transparent; "
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.5); }"
    );
    mainLayout->addWidget(lblTitle);

    m_lblBetAmount = new QLabel("Current Bet: $0", m_bettingWidget);
    m_lblBetAmount->setAlignment(Qt::AlignCenter);
    m_lblBetAmount->setStyleSheet(
        "QLabel { color: #00FFFF; font-size: 24px; font-weight: bold; background-color: transparent; "
        "text-shadow: 1px 1px 3px rgba(0,0,0,0.5); }"
    );
    mainLayout->addWidget(m_lblBetAmount);

    mainLayout->addSpacing(10);

    QHBoxLayout* chipsLayout = new QHBoxLayout();
    chipsLayout->setSpacing(20);

    QString chipStyle =
        "QPushButton { background-color: %1; color: white; border: 5px solid #FFD700; border-radius: 50px; "
        "font-size: 20px; font-weight: bold; min-width: 100px; min-height: 100px; max-width: 100px; max-height: 100px; "
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.7); }"
        "QPushButton:hover { border: 5px solid #FFFFFF; transform: scale(1.05); }"
        "QPushButton:pressed { background-color: %2; }"
        "QPushButton:disabled { background-color: #444444; border-color: #666666; color: #888888; }";

    m_btnBet25 = new QPushButton("$25", m_bettingWidget);
    m_btnBet25->setStyleSheet(chipStyle.arg("#1565C0").arg("#0D47A1"));
    connect(m_btnBet25, &QPushButton::clicked, this, [this]() { onBetButtonClicked(25); });
    chipsLayout->addWidget(m_btnBet25);

    m_btnBet50 = new QPushButton("$50", m_bettingWidget);
    m_btnBet50->setStyleSheet(chipStyle.arg("#43A047").arg("#2E7D32"));
    connect(m_btnBet50, &QPushButton::clicked, this, [this]() { onBetButtonClicked(50); });
    chipsLayout->addWidget(m_btnBet50);

    m_btnBet100 = new QPushButton("$100", m_bettingWidget);
    m_btnBet100->setStyleSheet(chipStyle.arg("#E53935").arg("#C62828"));
    connect(m_btnBet100, &QPushButton::clicked, this, [this]() { onBetButtonClicked(100); });
    chipsLayout->addWidget(m_btnBet100);

    m_btnBet250 = new QPushButton("$250", m_bettingWidget);
    m_btnBet250->setStyleSheet(chipStyle.arg("#8E24AA").arg("#6A1B9A"));
    connect(m_btnBet250, &QPushButton::clicked, this, [this]() { onBetButtonClicked(250); });
    chipsLayout->addWidget(m_btnBet250);

    m_btnBet500 = new QPushButton("$500", m_bettingWidget);
    m_btnBet500->setStyleSheet(chipStyle.arg("#FF8F00").arg("#EF6C00"));
    connect(m_btnBet500, &QPushButton::clicked, this, [this]() { onBetButtonClicked(500); });
    chipsLayout->addWidget(m_btnBet500);

    mainLayout->addLayout(chipsLayout);

    mainLayout->addSpacing(20);

    QHBoxLayout* actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(50);

    QString actionButtonStyle =
        "QPushButton { background-color: %1; color: white; border: 4px solid %2; border-radius: 15px; "
        "padding: 18px 60px; font-size: 24px; font-weight: bold; min-width: 180px; "
        "text-shadow: 3px 3px 6px rgba(0,0,0,0.8); }"
        "QPushButton:hover { background-color: %3; border-width: 5px; }"
        "QPushButton:pressed { padding: 20px 62px; }"
        "QPushButton:disabled { background-color: #444444; border-color: #666666; color: #888888; }";

    m_btnClearBet = new QPushButton("CLEAR", m_bettingWidget);
    m_btnClearBet->setStyleSheet(actionButtonStyle.arg("#C62828").arg("#FF5252").arg("#D32F2F"));
    m_btnClearBet->setFixedHeight(70);
    connect(m_btnClearBet, &QPushButton::clicked, this, &GameWindow::onClearBetClicked);
    actionLayout->addWidget(m_btnClearBet);

    m_btnDeal = new QPushButton("DEAL", m_bettingWidget);
    m_btnDeal->setStyleSheet(actionButtonStyle.arg("#2E7D32").arg("#66BB6A").arg("#388E3C"));
    m_btnDeal->setFixedHeight(70);
    m_btnDeal->setEnabled(false);
    connect(m_btnDeal, &QPushButton::clicked, this, &GameWindow::onDealClicked);
    actionLayout->addWidget(m_btnDeal);

    mainLayout->addLayout(actionLayout);

    repositionBettingWidget();
    m_bettingWidget->raise();
}

void GameWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    repositionBettingWidget();
}

void GameWindow::repositionBettingWidget()
{
    if (m_bettingWidget) {
        int widgetWidth = 750;
        int widgetHeight = 300;
        int x = (width() - widgetWidth) / 2;
        int y = (height() - widgetHeight) / 2 - 50;
        m_bettingWidget->setGeometry(x, y, widgetWidth, widgetHeight);
    }
}

void GameWindow::showBettingPhase()
{
    m_bettingPhase = true;
    m_currentBetAmount = 0;
    m_lblBetAmount->setText("Current Bet: $0");
    m_bettingWidget->show();
    m_bettingWidget->raise();
    repositionBettingWidget();

    m_btnHit->setEnabled(false);
    m_btnStand->setEnabled(false);
    m_btnDouble->setEnabled(false);
    m_btnSplit->setEnabled(false);
    m_btnInsurance->setEnabled(false);

    updateBettingButtons();
    updateBalanceDisplay();
}

void GameWindow::hideBettingPhase()
{
    m_bettingPhase = false;
    m_bettingWidget->hide();
}

void GameWindow::updateBettingButtons()
{
    int balance = m_game.getPlayer().getBalance();

    m_btnBet25->setEnabled(balance >= 25);
    m_btnBet50->setEnabled(balance >= 50);
    m_btnBet100->setEnabled(balance >= 100);
    m_btnBet250->setEnabled(balance >= 250);
    m_btnBet500->setEnabled(balance >= 500);

    m_btnClearBet->setEnabled(m_currentBetAmount > 0);
    m_btnDeal->setEnabled(m_currentBetAmount > 0);
}

void GameWindow::updateBalanceDisplay()
{
    const IPlayer& player = m_game.getPlayer();

    if (m_lblPlayerName)
        m_lblPlayerName->setText(QString("%1").arg(m_playerName));

    if (m_lblBalance)
        m_lblBalance->setText(QString("$%1").arg(player.getBalance()));

    if (m_lblCurrentBet) {
        if (m_bettingPhase)
            m_lblCurrentBet->setText(QString("Bet: $%1").arg(m_currentBetAmount));
        else
            m_lblCurrentBet->setText(QString("Bet: $%1").arg(player.getCurrentBet()));
    }

    if (m_lblInsuranceBet) {
        int ins = player.getInsuranceBet();
        if (ins > 0) {
            m_lblInsuranceBet->setText(QString("Ins: $%1").arg(ins));
            m_lblInsuranceBet->show();
        }
        else {
            m_lblInsuranceBet->hide();
        }
    }
}

void GameWindow::onBetButtonClicked(int amount)
{
    int balance = m_game.getPlayer().getBalance();

    if (m_currentBetAmount + amount <= balance) {
        m_currentBetAmount += amount;
        m_lblBetAmount->setText(QString("Current Bet: $%1").arg(m_currentBetAmount));
        m_game.getPlayer().addToBalance(-amount);
        updateBalanceDisplay();
        updateBettingButtons();
    }
    else {
        QMessageBox::warning(this, "Not Enough Balance",
            QString("You don't have enough balance to add $%1").arg(amount));
    }
}

void GameWindow::onClearBetClicked()
{
    m_game.getPlayer().addToBalance(m_currentBetAmount);
    m_currentBetAmount = 0;
    m_lblBetAmount->setText("Current Bet: $0");
    updateBalanceDisplay();
    updateBettingButtons();
}

void GameWindow::onDealClicked()
{
    if (m_currentBetAmount <= 0) {
        QMessageBox::warning(this, "No Bet", "Please place a bet before dealing!");
        return;
    }

    m_game.getPlayer().addToBalance(m_currentBetAmount);
    m_game.reset();
    m_game.requestBet(m_currentBetAmount);
    m_game.startGame();

    hideBettingPhase();
    updateUI();
}

void GameWindow::onHitClicked()
{
    m_game.hit();
    updateUI();

    GameState s = m_game.getState();
    if (s.gameOver) {
        showGameResult();
    }
}

void GameWindow::onStandClicked()
{
    m_game.stand();
    updateUI();

    GameState s = m_game.getState();
    if (s.gameOver) {
        showGameResult();
    }
}

void GameWindow::onDoubleClicked()
{
    if (!m_game.requestDoubleDown()) {
        QMessageBox::warning(this, "Cannot Double", "Not enough balance to double down!");
        return;
    }

    updateUI();

    GameState s = m_game.getState();
    if (s.gameOver) {
        showGameResult();
    }
}

void GameWindow::onSplitClicked()
{
    if (!m_game.requestSplit()) {
        QMessageBox::warning(this, "Cannot Split", "Split is not available right now!");
        return;
    }
    updateUI();
}

void GameWindow::onInsuranceClicked()
{
    GameState s = m_game.getState();

    if (!s.insuranceAllowed) {
        QMessageBox::information(this, "Insurance Info",
            "Insurance is only available when the dealer's visible card is an Ace.");
        return;
    }

    if (!m_game.requestInsurance()) {
        QMessageBox::warning(this, "Cannot Take Insurance", "Not enough balance for insurance bet!");
        return;
    }

    updateUI();

    if (m_game.getState().gameOver) {
        showGameResult();
    }
}

void GameWindow::onNewRoundClicked()
{
    if (m_game.getPlayer().getBalance() < 25) {
        QMessageBox::warning(this, "Game Over",
            QString("Not enough balance to continue!\n\n%1's final balance: $%2")
            .arg(m_playerName)
            .arg(m_game.getPlayer().getBalance()));
        return;
    }

    clearCardLayout(m_dealerCardsLayout);
    clearCardLayout(m_playerCardsLayout);
    clearCardLayout(m_playerSplitCardsLayout);

    if (m_splitHandContainer) {
        m_splitHandContainer->setVisible(false);
    }

    m_lblDealerTotal->setText("Dealer: ?");
    m_lblPlayerTotal->setText("Your Hand: ?");

    showBettingPhase();
}

void GameWindow::clearCardLayout(QHBoxLayout* layout)
{
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

QLabel* GameWindow::createCardLabel(const ICard* card, bool hidden) const
{
    QLabel* lbl = new QLabel();
    lbl->setFixedSize(100, 145);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (!card || hidden) {
        lbl->setText("?");
        lbl->setStyleSheet(
            "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
            "stop:0 #1a237e, stop:0.5 #283593, stop:1 #1a237e); "
            "color: #00FFFF; border: 3px solid #FFD700; border-radius: 12px; "
            "font-size: 42px; font-weight: bold; }"
        );
        return lbl;
    }

    QString suitSymbol;
    QString color;

    Suit suit = card->getSuit();
    switch (suit) {
    case Suit::Hearts:
        suitSymbol = QString::fromUtf8("♥");
        color = "#e53935";
        break;
    case Suit::Diamonds:
        suitSymbol = QString::fromUtf8("♦");
        color = "#e53935";
        break;
    case Suit::Clubs:
        suitSymbol = QString::fromUtf8("♣");
        color = "#212121";
        break;
    case Suit::Spades:
        suitSymbol = QString::fromUtf8("♠");
        color = "#212121";
        break;
    }

    QString valueStr;
    CardValue val = card->getValue();

    if (val == CardValue::Jack) {
        valueStr = "J";
    }
    else if (val == CardValue::Queen) {
        valueStr = "Q";
    }
    else if (val == CardValue::King) {
        valueStr = "K";
    }
    else if (val == CardValue::Ace) {
        valueStr = "A";
    }
    else {
        valueStr = QString::number(static_cast<int>(val));
    }

    QString displayText = QString("%1\n%2").arg(suitSymbol).arg(valueStr);
    lbl->setText(displayText);

    lbl->setStyleSheet(
        QString(
            "QLabel { background-color: #FFFEF0; color: %1; border: 2px solid #333333; "
            "border-radius: 12px; font-size: 30px; font-weight: bold; "
            "font-family: 'Georgia', 'Times New Roman', serif; }"
        ).arg(color)
    );

    return lbl;
}

void GameWindow::updateUI()
{
    GameState s = m_game.getState();

    m_btnHit->setEnabled(!s.gameOver && s.roundStarted);
    m_btnStand->setEnabled(!s.gameOver && s.roundStarted);
    m_btnDouble->setEnabled(s.doubleAllowed && !s.gameOver);
    m_btnSplit->setEnabled(s.splitAllowed && !s.gameOver);
    m_btnInsurance->setEnabled(s.insuranceAllowed && !s.gameOver);

    if (s.dealerHidden) {
        const IHand& dh = m_game.getDealerHand();
        if (dh.getCardCount() > 0) {
            const ICard* firstCard = dh.getCard(0);
            int visibleValue = static_cast<int>(firstCard->getValue());
            if (visibleValue > 10) visibleValue = 10;
            m_lblDealerTotal->setText(QString("Dealer: %1 + ?").arg(visibleValue));
        }
    }
    else {
        m_lblDealerTotal->setText(QString("Dealer: %1").arg(s.dealerTotal));
    }

    QString playerText = QString("Your Hand: %1").arg(s.mainTotal);
    if (s.splitActive) {
        playerText += QString(" | Split: %1").arg(s.splitTotal);
    }
    m_lblPlayerTotal->setText(playerText);

    clearCardLayout(m_dealerCardsLayout);
    const IHand& dh = m_game.getDealerHand();
    for (std::size_t i = 0; i < dh.getCardCount(); ++i) {
        const ICard* c = dh.getCard(i);
        bool hide = (s.dealerHidden && i == 1);
        QLabel* cardWidget = createCardLabel(c, hide);
        m_dealerCardsLayout->addWidget(cardWidget);
    }

    clearCardLayout(m_playerCardsLayout);
    const IHand& ph = m_game.getPlayerHand();
    for (std::size_t i = 0; i < ph.getCardCount(); ++i) {
        const ICard* c = ph.getCard(i);
        QLabel* cardWidget = createCardLabel(c, false);
        m_playerCardsLayout->addWidget(cardWidget);
    }

    if (s.splitActive && m_game.getPlayer().hasSplitHand()) {
        m_splitHandContainer->setVisible(true);
        clearCardLayout(m_playerSplitCardsLayout);
        const IHand& splitHand = m_game.getPlayer().getSplitHand();
        for (std::size_t i = 0; i < splitHand.getCardCount(); ++i) {
            const ICard* c = splitHand.getCard(i);
            QLabel* cardWidget = createCardLabel(c, false);
            m_playerSplitCardsLayout->addWidget(cardWidget);
        }
    }
    else {
        m_splitHandContainer->setVisible(false);
    }

    updateBalanceDisplay();
}

void GameWindow::showGameResult()
{
    GameState s = m_game.getState();

    if (!s.gameOver) return;

    QString message;
    QString title;

    const IPlayer& player = m_game.getPlayer();

    if (s.resultMain == GameResult::PlayerWon) {
        title = "YOU WIN!";
        message = QString("Congratulations %1!\n\nDealer: %2\nYour hand: %3\n\nCurrent Balance: $%4")
            .arg(m_playerName)
            .arg(s.dealerTotal)
            .arg(s.mainTotal)
            .arg(player.getBalance());
    }
    else if (s.resultMain == GameResult::DealerWon) {
        title = "DEALER WINS";
        message = QString("Better luck next time, %1!\n\nDealer: %2\nYour hand: %3\n\nCurrent Balance: $%4")
            .arg(m_playerName)
            .arg(s.dealerTotal)
            .arg(s.mainTotal)
            .arg(player.getBalance());
    }
    else if (s.resultMain == GameResult::Push) {
        title = "PUSH";
        message = QString("It's a tie, %1!\n\nDealer: %2\nYour hand: %3\n\nBet returned.\nCurrent Balance: $%4")
            .arg(m_playerName)
            .arg(s.dealerTotal)
            .arg(s.mainTotal)
            .arg(player.getBalance());
    }

    if (s.splitActive) {
        QString splitResultStr;
        if (s.resultSplit == GameResult::PlayerWon)
            splitResultStr = "Won";
        else if (s.resultSplit == GameResult::DealerWon)
            splitResultStr = "Lost";
        else
            splitResultStr = "Push";

        message += QString("\n\nSplit Hand: %1 (%2)")
            .arg(s.splitTotal)
            .arg(splitResultStr);
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
