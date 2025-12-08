#pragma once

#include <string>

class IObserver {
public:
    virtual void onNotify(const std::string& eventName) = 0;
    virtual ~IObserver() = default;
};


