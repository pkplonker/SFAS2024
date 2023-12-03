// The main Windows / DirectX Graphics / XInput entry point for Star Applications

#include <Windows.h>

#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Engine/Implementation/XInput/DirectXInput.h"
#include "Engine/IRenderable.h"
#include "Engine/IApplication.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "Logging/ConsoleSink.h"
#include "Logging/Debug.h"
#include "Logging/FileSink.h"
const char WindowClassName[] = "Star";
const char WindowTitle[] = "Stuart Heath SFAS24 - WIP";
const int WindowWidth = 2560;
const int WindowHeight = 1440;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
IApplication* GetEditorApplication(IGraphics* Graphics, IInput* Input, HWND hwnd);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IApplication* Application;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Debug::RegisterSink(new ConsoleSink());
    Debug::RegisterSink(new FileSink("S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/log.log"));

    Trace("Creating winman")
    WNDCLASSEX wc;
    HWND hwnd;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WindowClassName;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    Trace("Creating window")


    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                          CW_USEDEFAULT, WindowWidth, WindowHeight, nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr)
    {
        MessageBox(nullptr, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    Trace("Creating hwnd")

    ShowWindow(hwnd, nCmdShow);
    Trace("Showing window")

    UpdateWindow(hwnd);

    MSG msg;
    msg.message = WM_NULL;
    msg.wParam = -1;
    IGraphics* Graphics = new DirectX11Graphics(hwnd);
    IInput* Input = new DirectXInput();
    Application = GetEditorApplication(Graphics, Input, hwnd);
    if (Graphics && Graphics->IsValid() && Application)
    {
        Trace("Loading application")

        Application->Load();
        Trace("Application Loaded")
        Trace("Starting main loop")

        while (msg.message != WM_QUIT && Application->IsValid())
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            Input->Update();
            Application->Update();
            Graphics->Update();
            Application->PostGraphics();
            Graphics->PostUpdate();
        }
        Trace("Exiting loop")

        Application->Cleanup();
    }

    delete Application;
    delete Graphics;

    return static_cast<int>(msg.wParam);
}

void HandleWindowResize(int width, int height)
{
    if (Application) Application->Resize(width, height);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        HandleWindowResize(static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
