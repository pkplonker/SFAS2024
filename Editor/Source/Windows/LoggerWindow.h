#pragma once

#include "ImGuiWindow.h"

class BufferSink;
class ISink;
const std::string LOGGER = "Console";

class LoggerWindow : public EditorWindow
{
public:
    LoggerWindow(BufferSink* path);
    void Draw() override;
    std::string GetName() override;
private:
    BufferSink* sink;
};
