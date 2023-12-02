#include "LoggerWindow.h"

#include <fstream>
#include <ios>

#include "imgui.h"
#include "../Editor.h"
#include "../EditorSettings.h"
#include "../ImGuiHelpers.h"
#include "Logging/BufferSink.h"


LoggerWindow::LoggerWindow(BufferSink* sink) : sink(sink), currentLevel(0), collapse(false), showLine(false)
{
    currentLevel = EditorSettings::Get(CURRENTLEVEL_KEY, 0);
    collapse = EditorSettings::Get(COLLAPSE_KEY, false);
    showLine = EditorSettings::Get(SHOWLINE_KEY, false);
}

std::string LoggerWindow::CreateMessageString(const LogMessageData& line) const
{
    if (showLine)
    {
        return ISink::BeautifyLogLevel(line.level) + " " + line.message + ": " + line.file + ":" +
            std::to_string(line.line);
    }

    return ISink::BeautifyLogLevel(line.level) + " " + line.message;
}

LoggerWindow::~LoggerWindow()
{
    EditorSettings::Set(CURRENTLEVEL_KEY, currentLevel);
    EditorSettings::Set(COLLAPSE_KEY, collapse);
    EditorSettings::Set(SHOWLINE_KEY, showLine);
}

void LoggerWindow::Draw()
{
    ImGui::Begin("Console");
    if (ImGui::Button("Test Log"))
    {
    }
    ImGui::SameLine();
    ImGui::Checkbox("Collapse", &collapse);
    ImGui::SameLine();
    ImGui::Checkbox("Show File/Line", &showLine);

    ImGui::SameLine();
    if (ImGui::Button("Clear Log"))
    {
        sink->Clear();
    }
    ImGui::SameLine();
    ImGui::Text("Level Filter");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.0f);

    ImGui::Combo("##combobox", &currentLevel, levels, IM_ARRAYSIZE(levels));
    ImGui::SameLine();
    ImGui::Text("Text Filter");
    ImGui::SameLine();
    filter.Draw("##Filter", 180.0f);
    ImGui::SameLine();
    if (ImGui::Button("Clear Filter"))
    {
        filter.Clear();
    }
    ImGui::BeginChild("Scrolling");
    if (collapse)
    {
        std::unordered_map<std::string, int> messageCounts;
        std::vector<std::string> orderedMessages;

        for (const auto& line : sink->GetBuffer())
        {
            if (line.level >= currentLevel)
            {
                std::string message = CreateMessageString(line);
                if (messageCounts.find(message) == messageCounts.end())
                {
                    orderedMessages.push_back(message);
                }
                messageCounts[message]++;
            }
        }

        for (const auto& message : orderedMessages)
        {
            if (filter.PassFilter(message.c_str()))
            {
                ImGui::Text("%s (%d)", message.c_str(), messageCounts[message]);
            }
        }
    }
    else
    {
        for (const auto& line : sink->GetBuffer())
        {
            if (line.level >= currentLevel)
            {
                std::string message = CreateMessageString(line);
                if (filter.PassFilter(message.c_str()))
                {
                    ImGui::TextUnformatted(message.c_str());
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

std::string LoggerWindow::GetName()
{
    return LOGGER;
}
