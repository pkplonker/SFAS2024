#include "UndoManager.h"

#include <iostream>
#include <ostream>

#include "IInput.h"

UndoManager::UndoManager(IInput* input)
{
    this->input = input;
}

void UndoManager::Undo()
{
    std::cout << "pressed" << std::endl;
}

void UndoManager::Redo()
{
    std::cout << "pressed redo"  << std::endl;
}

void UndoManager::Update()
{
    if (input->IsKeyDown(Keys::LeftControl))
    {
        if (input->IsKeyPress(Keys::Z))
        {
            Undo();
        }
        else if (input->IsKeyPress(Keys::Y))
        {
            Redo();
        }
    }
}
