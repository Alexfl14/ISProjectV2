#include "GameFactory.h"
#include "Game.h"
#include "StandardDealerStrategy.h"
#include "HitOnSoft17Strategy.h"
#include <memory>

IGame* GameFactory::createGame() {
    return new Game();
}

IGame* GameFactory::createGameWithStandardRules() {
    Game* game = new Game();
    game->setDealerStrategy(std::make_unique<StandardDealerStrategy>());
    return game;
}

IGame* GameFactory::createGameWithSoft17Rules() {
    Game* game = new Game();
    game->setDealerStrategy(std::make_unique<HitOnSoft17Strategy>());
    return game;
}
