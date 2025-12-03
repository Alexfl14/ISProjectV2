#pragma once
#include "interfaces/IGameFactory.h"

class GameFactory : public IGameFactory {
public:
    IGame* createGame() override;
    
    IGame* createGameWithStandardRules();
    IGame* createGameWithSoft17Rules();
};
