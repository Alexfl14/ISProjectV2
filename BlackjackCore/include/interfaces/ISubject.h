#pragma once

#include <string>
class IObserver;

class ISubject {
public:
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers(const std::string& eventName) = 0;

    virtual ~ISubject() = default;
};