#include "Memento.h"

Memento::Memento(std::function<void()> action, std::function<void()> undo) : Memento(action, undo, DEFAULT_DESCRIPTION)
{
}

Memento::Memento(std::function<void()> action, std::function<void()> undo, std::string description)
{
    this->action = action;
    this->undo = undo;
    this->description = description;
}

void Memento::Execute()
{
    if (action != nullptr)
    {
        action();
    }
}

void Memento::Undo()
{
    if (undo != nullptr)
    {
        undo();
    }
}
