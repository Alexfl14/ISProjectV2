#pragma once

#include "enums/EGameResult.h"

struct GameState {
    int mainTotal = 0;
    int splitTotal = -1;    
    int dealerTotal = 0;

    
    bool splitAllowed = false;       
    bool doubleAllowed = false;      
    bool insuranceAllowed = false;   
    bool splitActive = false;        
    bool roundStarted = false;       
    bool gameOver = false;           
    bool dealerHidden = false;

   
    EGameResult resultMain = EGameResult::None;
    EGameResult resultSplit = EGameResult::None;
};
