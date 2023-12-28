#pragma once
#include <stack>

#include "IUpdateable.h"
#include "Memento.h"

class IInput;

class UndoManager : public IUpdateable
{
public:
    UndoManager(IInput* input);
    static void Execute(Memento command, bool execute = true);
    static void Undo();
    static void Redo();
    void Update() override;

    ~UndoManager() override
    {
    }

    static std::vector<std::string> GetCommandDescriptions();

private:
    IInput* input = {};
   static inline bool isDirty;
    static inline bool init = false;;
    static inline std::stack<Memento> commands = {};
    static inline std::vector<std::string> commandStrings = {};
    static inline std::stack<Memento> redoCommands = {};

};
