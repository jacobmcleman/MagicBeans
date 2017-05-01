// TestApplication.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestApplication.h"

#include <MagicBeansEngine.h>

#define MAX_LOADSTRING 100

#include "Logger.h"
#include "TestPlayerController.h"
#include "CubeMesh.h"
#include "LookAtMeComponent.h"
#include "FPSController.h"

using namespace Beans;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RestartLog();

    MagicBeansEngine testApp("TestApplication");

    testApp.RegisterUpdateFunction(PlayerController::UpdatePlayerControllers);
    testApp.RegisterUpdateFunction(LookAtMe::UpdateLookAtMes);
    testApp.RegisterUpdateFunction(FPSController::UpdateFPSControllers);

    GameObject* player1 = testApp.CreateObject("Player");

    float floorHeight = -5;
    float ceilingHeight = 50;

    float roomHeight = ceilingHeight - floorHeight;
    float wallCenter = floorHeight + (roomHeight / 2);
    float wallThickness = 1;
    float roomWidth = 100;
    float roomLength = 100;

    vec3 playerScale(10, 10, 10);

    vec3 roomColor(0.9f, 0.9f, 0.9f);

    Transform* transform = player1->AddComponent<Transform>();
    //PlayerController* pc = player1->AddComponent<PlayerController>();
    //LookAtMe* lam = player1->AddComponent<LookAtMe>();
    //lam->SetCamera(testApp.GetCamera()->GetComponent<Camera>());
    transform->position = glm::vec3(10, 0, floorHeight + (playerScale.z / 2));
    transform->scale = glm::vec3(10, 10, 10);
    CubeMesh* cube = player1->AddComponent<CubeMesh>();
    cube->Color = vec3(1, 0.05f, 0.05f);

    GameObject* floor = testApp.CreateObject("Floor");
    transform = floor->AddComponent<Transform>();
    transform->position = vec3(0, 0, floorHeight);
    transform->scale = vec3(roomWidth, roomLength, 1);
    cube = floor->AddComponent<CubeMesh>();
    cube->Color = roomColor;

    GameObject* roof = testApp.CreateObject("Roof");
    transform = roof->AddComponent<Transform>();
    transform->position = vec3(0, 0, ceilingHeight);
    transform->scale = vec3(roomWidth, roomLength, 1);
    cube = roof->AddComponent<CubeMesh>();
    cube->Color = roomColor;

    GameObject* northWall = testApp.CreateObject("North Wall");
    transform = northWall->AddComponent<Transform>();
    transform->position = vec3(0, 0.5f * roomLength, wallCenter);
    transform->scale = vec3(roomWidth, wallThickness, roomHeight);
    cube = northWall->AddComponent<CubeMesh>();
    cube->Color = roomColor;

    GameObject* southWall = testApp.CreateObject("South Wall");
    transform = southWall->AddComponent<Transform>();
    transform->position = vec3(0, -0.5f * roomLength, wallCenter);
    transform->scale = vec3(roomWidth, wallThickness, roomHeight);
    cube = southWall->AddComponent<CubeMesh>();
    cube->Color = roomColor;

    GameObject* westWall = testApp.CreateObject("West Wall");
    transform = westWall->AddComponent<Transform>();
    transform->position = vec3(-0.5f * roomWidth, 0, wallCenter);
    transform->scale = vec3(wallThickness, roomLength, roomHeight);
    cube = westWall->AddComponent<CubeMesh>();
    cube->Color = roomColor;

    GameObject* eastWall = testApp.CreateObject("East Wall");
    transform = eastWall->AddComponent<Transform>();
    transform->position = vec3(0.5f * roomWidth, 0, wallCenter);
    transform->scale = vec3(wallThickness, roomLength, roomHeight);
    cube = eastWall->AddComponent<CubeMesh>();
    cube->Color = roomColor;
    
    //transform->rotation = 4.0f;
    GameObject* cam = testApp.GetCamera();
    cam->GetComponent<Transform>()->position = vec3(0, 0, wallCenter);
    cam->AddComponent<FPSController>();

    testApp.RunGameLoop();

    return 0;
}