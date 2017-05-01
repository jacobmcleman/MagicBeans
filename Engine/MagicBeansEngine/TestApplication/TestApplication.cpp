// TestApplication.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestApplication.h"

#include <MagicBeansEngine.h>

#define MAX_LOADSTRING 100

#include "Logger.h"
#include "TestPlayerController.h"

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

    GameObject* player1 = testApp.CreateObject("Player");

    Transform* transform = player1->AddComponent<Transform>();
    PlayerController* pc = player1->AddComponent<PlayerController>();

    transform->position = glm::vec3(10, 0, 1);
    transform->scale = glm::vec3(10, 10, 10);
    //transform->rotation = 4.0f;

    testApp.GetCamera()->GetComponent<Transform>()->position = vec3(0, 20, 55);

    testApp.RunGameLoop();

    return 0;
}