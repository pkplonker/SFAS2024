#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include <ctime>

#include "Engine/Debug.h"
#include "Engine/GameObject.h"
#include "Engine/Implementation/SpriteRenderable.h"
#include "Engine/Implementation/TestComponent.h"

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = Pie / 10.0f;

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
    return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), Rings(), Arrow(nullptr),
                                                     SelectedRing(), State()
{
}

Game::~Game()
{
}

bool Game::IsValid()
{
    return true;
}

bool Game::Load()
{
    ITexture* InnerTexture = Graphics->CreateTexture(L"Resource/Textures/InnerRing.dds");
    ITexture* MiddleTexture = Graphics->CreateTexture(L"Resource/Textures/MiddleRing.dds");
    ITexture* OuterTexture = Graphics->CreateTexture(L"Resource/Textures/OuterRing.dds");
    ITexture* ArrowTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow.dds");
    IShader* InnerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main",
                                                  "ps_4_0", InnerTexture);
    IShader* MiddleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main",
                                                   "ps_4_0", MiddleTexture);
    IShader* OuterShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main",
                                                  "ps_4_0", OuterTexture);
    IShader* ArrowShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main",
                                                  "ps_4_0", ArrowTexture);

    std::shared_ptr<GameObject> inner = std::make_shared<GameObject>("Inner Ring");
    std::shared_ptr<GameObject> middle = std::make_shared<GameObject>("Middle Ring");
    std::shared_ptr<GameObject> outer = std::make_shared<GameObject>("Outer Ring");
    Arrow = std::make_shared<GameObject>("Arrow");

    std::shared_ptr<SpriteRenderable> sri = std::make_shared<SpriteRenderable>(
        inner, Graphics->CreateBillboard(InnerShader));
    std::shared_ptr<SpriteRenderable> srm = std::make_shared<SpriteRenderable>(
        middle, Graphics->CreateBillboard(MiddleShader));
    std::shared_ptr<SpriteRenderable> sro = std::make_shared<SpriteRenderable>(
        outer, Graphics->CreateBillboard(OuterShader));
    std::shared_ptr<SpriteRenderable> sra = std::make_shared<SpriteRenderable>(
        Arrow, Graphics->CreateBillboard(ArrowShader));

    inner->AddComponent(sri);
    middle->AddComponent(srm);
    outer->AddComponent(sro);
    Arrow->AddComponent(sra);
    inner->Transform()->Position.Z(10);
    middle->Transform()->Position.Z(10);
    outer->Transform()->Position.Z(10);
    Arrow->Transform()->Position.Z(10);

    Rings[static_cast<unsigned int>(Inner)] = inner;
    Rings[static_cast<unsigned int>(Middle)] = middle;
    Rings[static_cast<unsigned int>(Outer)] = outer;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SelectedRing = RingLayer::Outer;
    State = GameState::Setup;

    return true;
}

void Game::Update()
{
    // If mode is Setup game then set each ring to a random rotation
    if (State == GameState::Setup)
    {
        SetupEachRing();
        State = GameState::Playing;
    }

    // If mode is Playing then read controller input and manage which ring is selected, the rotation of each ring and waiting for select to confirm positions
    if (State == GameState::Playing)
    {
        UpdateRingSelection();
        UpdateSelectedRingRotation();
        UpdateRingTestSelection();
    }

    // If mode is Test then check to see if the rings are in their correct positions, play a noise corresponding to how close the player is
    if (State == GameState::Test)
    {
        TestRingSolution();
        State = GameState::Setup;
    }
}

void Game::Cleanup()
{
}

void Game::SetupEachRing()
{
    for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
    {
        Rings[Ring]->Transform()->Rotation = Vec3(0,0,static_cast<float>(fmod(rand(), Pie)));
    }

    Arrow->Transform()->Rotation = Vec3(0,0,static_cast<float>(fmod(rand(), Pie)));
}

void Game::UpdateRingSelection()
{
    int selectionChange = 0;

    if (Input->IsPressed(InputAction::ShoulderButtonLeft))
    {
        // Change ring selection towards outer
        selectionChange = -1;
    }
    else if (Input->IsPressed(InputAction::ShoulderButtonRight))
    {
        // Change ring selection towards inner
        selectionChange = 1;
    }

    SelectedRing = static_cast<RingLayer>(
        CLAMP(static_cast<int>(SelectedRing) + selectionChange, 0, NumberOfRings - 1));
}

void Game::UpdateSelectedRingRotation()
{
    float delta = Input->GetValue(InputAction::RightStickXAxis) * SpinSpeed * DeltaTime;
    Vec3 rotation = Rings[static_cast<int>(SelectedRing)]->Transform()->Rotation;
    Vec3 newRotation = Vec3(0,0, static_cast<float>(fmod(rotation.Z() + delta, TwoPies)));
    Rings[static_cast<int>(SelectedRing)]->Transform()->Rotation = newRotation;
}

void Game::UpdateRingTestSelection()
{
    if (Input->IsPressed(InputAction::DefaultSelect))
    {
        State = GameState::Test;
    }
}

void Game::TestRingSolution()
{
    float totalRotationDifference = 0.0f;
    float arrowRotation = Arrow->Transform()->Rotation.Z() + TwoPies;

    for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
    {
        totalRotationDifference += abs(arrowRotation - (Rings[Ring]->Transform()->Rotation.Z() + TwoPies));
    }

    float averageRotationDifference = totalRotationDifference / NumberOfRings;

    if (averageRotationDifference < WinTolerance)
    {
        Debug("win")
    }
    else
    {
        Debug("Lose")
    }
}
