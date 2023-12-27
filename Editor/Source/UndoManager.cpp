#include "UndoManager.h"

#include <iostream>
#include <ostream>
#include "Memento.h"
#include "IInput.h"

UndoManager::UndoManager(IInput* input)
{
    this->input = input;
    init = true;
}

void UndoManager::Execute(Memento command)
{
    if (!init) throw;
    isDirty = true;
    commands.emplace(command);
    commands.top().Execute();
    std::cout << "Executed " << commands.top().description << std::endl;
}

void UndoManager::Undo()
{
    if (!init || commands.empty()) return;
    isDirty = true;
    auto command = commands.top();
    command.Undo();
    commands.pop();
    std::cout << "Undone " << command.description << std::endl;
}

void UndoManager::Redo()
{
    if (!init || commands.empty()) return;
    isDirty = true;
    std::cout << "pressed redo" << std::endl;
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

std::vector<std::string> UndoManager::GetCommandDescriptions()
{
    if (!init) return std::vector<std::string>();
    if (!isDirty) return commandStrings;
    std::stack<Memento> tempStack = commands;
    commandStrings.clear();
    while (!tempStack.empty())
    {
        const Memento& command = tempStack.top();
        commandStrings.push_back(command.description);
        tempStack.pop();
    }

    std::reverse(commandStrings.begin(), commandStrings.end());
    isDirty=false;
    return commandStrings;
}
