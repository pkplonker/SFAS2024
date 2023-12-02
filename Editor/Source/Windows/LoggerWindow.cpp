#include "LoggerWindow.h"

#include <fstream>
#include <ios>

#include "imgui.h"
#include "imgui_internal.h"
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



void LoggerWindow::Draw()
{
    ImGui::Begin("Console");
    ImGui::SameLine();
    ImGui::Text("Collapse:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##Collapse", &collapse))
    {
        EditorSettings::Set(COLLAPSE_KEY, collapse);
    }
    ImGui::SameLine();
    ImGui::Text("Show File/Line:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##Show File/Line", &showLine))
    {
        EditorSettings::Set(SHOWLINE_KEY, showLine);
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear Log"))
    {
        sink->Clear();
    }
    ImGui::SameLine();
    ImGui::Text("Level Filter:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.0f);

    if (ImGui::Combo("##combobox", &currentLevel, levels, IM_ARRAYSIZE(levels)))
    {
        EditorSettings::Set(CURRENTLEVEL_KEY, currentLevel);
    }
    ImGui::SameLine();
    ImGui::Text("Text Filter:");
    ImGui::SameLine();
    filter.Draw("##Filter", 180.0f);
    ImGui::SameLine();
    if (ImGui::Button("Clear Filter"))
    {
        filter.Clear();
    }
    ImGui::BeginChild("Scrolling", ImVec2(0, 0), true,
                      ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (collapse)
    {
        std::unordered_map<std::string, int> messageCounts;
        std::vector<std::pair<std::string, LogMessageData>> orderedMessages;

        for (const auto& line : sink->GetBuffer())
        {
            if (line.level >= currentLevel)
            {
                std::string message = CreateMessageString(line);
                if (messageCounts.find(message) == messageCounts.end())
                {
                    orderedMessages.push_back(std::pair(message, line));
                }
                messageCounts[message]++;
            }
        }

        for (const auto& message : orderedMessages)
        {
            if (filter.PassFilter(message.first.c_str()))
            {
                if (message.second.level == 2)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                    ImGui::Text("%s (%d)", message.first.c_str(), messageCounts[message.first]);
                    ImGui::PopStyleColor();
                }
                else if (message.second.level == 3)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                    ImGui::Text("%s (%d)", message.first.c_str(), messageCounts[message.first]);
                    ImGui::PopStyleColor();
                }
                else
                {
                    ImGui::Text("%s (%d)", message.first.c_str(), messageCounts[message.first]);
                }
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
                    if (line.level == 2)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                        ImGui::Text(message.c_str());
                        ImGui::PopStyleColor();
                    }
                    else if (line.level == 3)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                        ImGui::Text(message.c_str());
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::Text(message.c_str());
                    }
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
