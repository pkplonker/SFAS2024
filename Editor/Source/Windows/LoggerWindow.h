#pragma once

#include <unordered_map>

#include "imgui.h"
#include "EditorWindow.h"
#include "Logging/BufferSink.h"
#include "Logging/Debug.h"

class BufferSink;
class ISink;
const std::string LOGGER = "Console";

const std::string CURRENTLEVEL_KEY = "CURRENTLEVEL_KEY";
const std::string COLLAPSE_KEY = "COLLAPSE_KEY";
const std::string SHOWLINE_KEY = "SHOWLINE_KEY";
const std::string ENABLED_KEY = "ENABLED_KEY";

class LoggerWindow : public EditorWindow
{
public:
    LoggerWindow(BufferSink* path);
    std::string CreateMessageString(const LogMessageData& line) const;
    ~LoggerWindow() override = default;
    void Draw() override;
    std::string GetName() override;

private:
    BufferSink* sink;
    const char* levels[4] = {"Trace", "Info", "Warning", "Error"};
    int currentLevel;
    bool collapse;
    bool showLine;
    ImGuiTextFilter filter;
    bool isEnabled;
    void CacheLogMessages();
    void PrintMessageLine(const std::vector<std::pair<std::string, LogMessageData>>::value_type& message);
    std::unordered_map<std::string, int> cachedMessageCounts;
    std::vector<std::pair<std::string, LogMessageData>> cachedOrderedMessages;
    bool isLocalDirty;
};
