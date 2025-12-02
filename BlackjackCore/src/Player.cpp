#include "Player.h"

Player::Player(int initialBalance)
    : balance(initialBalance), currentBet(0), insuranceBet(0) {
}

int Player::getBalance() const { return balance; }
int Player::getCurrentBet() const { return currentBet; }
int Player::getInsuranceBet() const { return insuranceBet; }

void Player::addToBalance(int amount) {
    balance += amount;
}

bool Player::bet(int amount) {
    if (amount <= 0) return false;
    if (currentBet > 0)
        balance += currentBet;
    if (amount > balance) return false;
    balance -= amount;
    currentBet = amount;
    insuranceBet = 0;
    return true;
}

void Player::clearBet() {
    if (currentBet > 0)
        balance += currentBet;
    currentBet = 0;
    insuranceBet = 0;
}

bool Player::takeInsurance() {
    if (currentBet <= 0) return false;
    if (insuranceBet > 0) return false;
    int required = currentBet / 2;
    if (required > balance) return false;
    balance -= required;
    insuranceBet = required;
    return true;
}

bool Player::doubleDown() {
    if (currentBet <= 0) return false;
    if (balance < currentBet) return false;
    balance -= currentBet;
    currentBet *= 2;
    return true;
}

bool Player::canSplit() const {
    if (mainHand.getCardCount() != 2)
        return false;
    const ICard* c1 = mainHand.getCard(0);
    const ICard* c2 = mainHand.getCard(1);
    if (!c1 || !c2) return false;
    if (c1->getValue() != c2->getValue()) return false;
    return balance >= currentBet;
}

void Player::performSplit() {
    if (!canSplit()) return;
    balance -= currentBet;
    currentBet *= 2;
    const ICard* second = mainHand.getCard(1);
    splitHand.clear();
    splitHand.addCard(std::unique_ptr<ICard>(second->clone()));
    mainHand.removeCardAt(1);
    splitActive = true;
}

bool Player::hasSplitHand() const { return splitActive; }

IHand& Player::getMainHand() { return mainHand; }
IHand& Player::getSplitHand() { return splitHand; }
const IHand& Player::getMainHand() const { return mainHand; }
const IHand& Player::getSplitHand() const { return splitHand; }

void Player::win() {
    if (currentBet > 0) {
        balance += currentBet * 2;
        currentBet = 0;
    }
}

void Player::blackjackWin() {
    if (currentBet > 0) {
        balance += static_cast<int>(currentBet * 2.5);
        currentBet = 0;
    }
}

void Player::push() {
    if (currentBet > 0) {
        balance += currentBet;
        currentBet = 0;
    }
}

void Player::lose() {
    currentBet = 0;
}

void Player::resolveInsurance(bool dealerHasBlackjack) {
    if (insuranceBet <= 0) return;
    if (dealerHasBlackjack)
        balance += insuranceBet * 2;
    insuranceBet = 0;
}

void Player::resetForNewRound() {
    clearBet();
    mainHand.clear();
    splitHand.clear();
    splitActive = false;
}
