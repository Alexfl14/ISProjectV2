#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "Game.h"
#include "interfaces/ICard.h"
#include "interfaces/IHand.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(const QString& playerName = "Player", QWidget* parent = nullptr);
    ~GameWindow();

signals:
    void returnToMenu();

private slots:
    void onHitClicked();
    void onStandClicked();
    void onDoubleClicked();
    void onSplitClicked();
    void onInsuranceClicked();
    void onNewRoundClicked();
    void onBetButtonClicked(int amount);
    void onClearBetClicked();
    void onDealClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void setupTableBackground();
    void setupInfoLabels();
    void setupGameControls();
    void setupBettingWidget();
    void repositionBettingWidget();
    void showBettingPhase();
    void hideBettingPhase();
    void updateUI();
    void updateBalanceDisplay();
    void updateBettingButtons();
    void showGameResult();
    void clearCardLayout(QHBoxLayout* layout);
    QLabel* createCardLabel(const ICard* card, bool hidden = false) const;
    QWidget* createVerticalControlsPanel();

    Game m_game;
    QString m_playerName;
    int m_currentBetAmount;
    bool m_bettingPhase;

    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_dealerCardsLayout;
    QHBoxLayout* m_playerCardsLayout;
    QHBoxLayout* m_playerSplitCardsLayout;

    QWidget* m_splitHandContainer;
    QLabel* m_lblSplitHandTitle;

    QLabel* m_lblPlayerName;
    QLabel* m_lblBalance;
    QLabel* m_lblCurrentBet;
    QLabel* m_lblInsuranceBet;
    QLabel* m_lblDealerTitle;
    QLabel* m_lblDealerTotal;
    QLabel* m_lblPlayerTitle;
    QLabel* m_lblPlayerTotal;

    QPushButton* m_btnHit;
    QPushButton* m_btnStand;
    QPushButton* m_btnDouble;
    QPushButton* m_btnSplit;
    QPushButton* m_btnInsurance;
    QPushButton* m_btnNewRound;

    QWidget* m_bettingWidget;
    QPushButton* m_btnBet25;
    QPushButton* m_btnBet50;
    QPushButton* m_btnBet100;
    QPushButton* m_btnBet250;
    QPushButton* m_btnBet500;
    QPushButton* m_btnClearBet;
    QPushButton* m_btnDeal;
    QLabel* m_lblBetAmount;
};

#endif