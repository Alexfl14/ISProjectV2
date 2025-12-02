#include "GameFactory.h"
#include "Game.h"

IGame* GameFactory::createGame() {
    return new Game();
}
