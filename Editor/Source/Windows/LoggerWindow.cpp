#include "LoggerWindow.h"

#include <iostream>

#include "imgui.h"
#include "../Editor.h"
#include "../EditorSettings.h"
#include "../ImGuiHelpers.h"
#include "Logging/BufferSink.h"


LoggerWindow::LoggerWindow(BufferSink* sink) : sink(sink), currentLevel(0), collapse(false), showLine(false),
                                               isLocalDirty(true)
{
    currentLevel = EditorSettings::Get(CURRENTLEVEL_KEY, 0);
    collapse = EditorSettings::Get(COLLAPSE_KEY, false);
    showLine = EditorSettings::Get(SHOWLINE_KEY, false);
    isEnabled = EditorSettings::Get(ENABLED_KEY, true);
}

std::string LoggerWindow::CreateMessageString(const LogMessageData& line) const
{
    std::ostringstream messageStream;
    messageStream << ISink::BeautifyLogLevel(line.level) << " " << line.message;

    if (showLine)
    {
        messageStream << ": " << line.file << ":" << line.line;
    }

    return messageStream.str();
}

void LoggerWindow::CacheLogMessages()
{
    cachedMessageCounts.clear();
    cachedOrderedMessages.clear();

    if (collapse)
    {
        for (const auto& line : sink->GetBuffer())
        {
            if (line.level >= currentLevel)
            {
                std::string message = CreateMessageString(line);
                if (cachedMessageCounts.find(message) == cachedMessageCounts.end())
                {
                    cachedOrderedMessages.push_back(std::make_pair(message, line));
                }
                cachedMessageCounts[message]++;
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
                cachedOrderedMessages.push_back(std::make_pair(message, line));
            }
        }
    }

    isLocalDirty = false;
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
        isLocalDirty = true;
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
        cachedMessageCounts.clear();
        cachedOrderedMessages.clear();
        isLocalDirty = true;
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
    ImGui::SameLine();
    if (ImGui::Checkbox("##Enabled", &isEnabled))
    {
        EditorSettings::Set(ENABLED_KEY, isEnabled);
        isLocalDirty = true;
    }
    ImGui::SameLine();
    ImGui::Text("Enabled?");
    ImGui::SameLine();

    if (sink->IsDirty() || isLocalDirty && isEnabled)
    {
        CacheLogMessages();
        sink->ClearDirty();
    }

    ImGui::BeginChild("Scrolling", ImVec2(0, 0), true,
                      ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (isEnabled)
    {
        for (const auto& message : cachedOrderedMessages)
        {
            if (filter.PassFilter(message.first.c_str()))
            {
                if (message.second.level == 2)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                    ImGui::Text("%s (%d)", message.first.c_str(), cachedMessageCounts[message.first]);
                    ImGui::PopStyleColor();
                }
                else if (message.second.level == 3)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                    ImGui::Text("%s (%d)", message.first.c_str(), cachedMessageCounts[message.first]);
                    ImGui::PopStyleColor();
                }
                else
                {
                    ImGui::Text("%s (%d)", message.first.c_str(), cachedMessageCounts[message.first]);
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
