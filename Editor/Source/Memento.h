#pragma once
#include <functional>
#include <string>

class Memento
{
public:
    Memento(std::function<void()> action, std::function<void()> undo);
    Memento(std::function<void()> Do, std::function<void()> undo, std::string description);
    void Execute();
    void Undo();

private:
    std::function<void()> action;
    std::function<void()> undo;
    std::string description;
    inline static const std::string DEFAULT_DESCRIPTION = "Performed action";
};
