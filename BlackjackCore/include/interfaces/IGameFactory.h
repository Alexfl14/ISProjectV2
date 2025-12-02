#pragma once
#include "IGame.h"

class IGameFactory {
public:
    virtual IGame* createGame() = 0;
    virtual ~IGameFactory() = default;
};
