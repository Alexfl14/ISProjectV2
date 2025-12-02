#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget* parent = nullptr);
    ~MainMenu();

signals:
    void startNewGame(const QString& playerName);
    void continueGame();
    void quitRequested();

private slots:
    void onNewGameClicked();
    void onContinueClicked();
    void onQuitClicked();

private:
    void setupUI();

    QLabel* m_lblTitle;
    QLabel* m_lblSubtitle;
    QLabel* m_lblNamePrompt;
    QLineEdit* m_txtPlayerName;
    QPushButton* m_btnNewGame;
    QPushButton* m_btnQuit;
};

#endif