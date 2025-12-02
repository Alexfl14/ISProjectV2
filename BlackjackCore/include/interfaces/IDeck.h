#pragma once

#include "ICard.h"
#include <memory>

class IDeck {
public:
    virtual void shuffle() = 0;                   
    virtual std::unique_ptr<ICard> draw() = 0; 
    virtual int size() const = 0;             

    virtual ~IDeck() = default;
};
