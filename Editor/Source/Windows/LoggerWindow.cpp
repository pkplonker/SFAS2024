#include "LoggerWindow.h"

#include <fstream>
#include <ios>

#include "imgui.h"
#include "Logging/BufferSink.h"

LoggerWindow::LoggerWindow(BufferSink* sink) : sink(sink)
{
}


void LoggerWindow::Draw()
{
    ImGui::Begin("Console");

    ImGui::BeginChild("Scrolling");
    for (const auto& line : sink->GetBuffer())
    {
        ImGui::TextUnformatted(line.c_str());
    }
    ImGui::EndChild();

    ImGui::End();
}

std::string LoggerWindow::GetName()
{
    return LOGGER;
}
