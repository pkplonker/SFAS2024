#pragma once

#include "imgui.h"
#include "ImGuiWindow.h"
#include "Logging/BufferSink.h"
#include "Logging/Debug.h"

class BufferSink;
class ISink;
const std::string LOGGER = "Console";

const std::string CURRENTLEVEL_KEY;
const std::string COLLAPSE_KEY;
const std::string SHOWLINE_KEY;

class LoggerWindow : public EditorWindow
{
public:
    LoggerWindow(BufferSink* path);
    std::string CreateMessageString(const LogMessageData& line) const;
    ~LoggerWindow();
    void Draw() override;
    std::string GetName() override;

private:
    BufferSink* sink;
    const char* levels[4] = {"Trace", "Info", "Warning", "Error"};
    int currentLevel;
    bool collapse;
    bool showLine;
    ImGuiTextFilter filter;

};
