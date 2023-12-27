#pragma once
#include <stack>

#include "IUpdateable.h"
#include "Memento.h"

class IInput;

class UndoManager : public IUpdateable
{
public:
    UndoManager(IInput* input);
    void Undo();
    void Redo();
    void Update() override;
    ~UndoManager() override{}
private:
    IInput* input;
    std::stack<Memento> mementos;
};
