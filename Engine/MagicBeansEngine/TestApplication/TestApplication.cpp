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
#include "PointLight.h"
#include "IdleMovement.h"

using namespace Beans;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RestartLog();

    MagicBeansEngine testApp("TestApplication", CursorMode::Disabled);

    testApp.RegisterUpdateFunction(PlayerController::UpdatePlayerControllers);
    testApp.RegisterUpdateFunction(LookAtMe::UpdateLookAtMes);
    testApp.RegisterUpdateFunction(FPSController::UpdateFPSControllers);
    testApp.RegisterUpdateFunction(IdleMovement::UpdateIdleMovers);

    GameObject* player1 = testApp.CreateObject("Player");

    float floorHeight = -5;
    float ceilingHeight = 30;

    float roomHeight = ceilingHeight - floorHeight;
    float wallCenter = floorHeight + (roomHeight / 2);
    float wallThickness = 1;
    float roomWidth = 100;
    float roomLength = 100;

    vec3 playerScale(10, 10, 10);

    Material roomMat = Material::Plastics::White;

    Transform* transform = player1->AddComponent<Transform>();
    //PlayerController* pc = player1->AddComponent<PlayerController>();
    //LookAtMe* lam = player1->AddComponent<LookAtMe>();
    //lam->SetCamera(testApp.GetCamera()->GetComponent<Camera>());
    transform->position = glm::vec3(10, 0, floorHeight + (playerScale.z / 2));
    transform->scale = glm::vec3(10, 10, 10);
    CubeMesh* cube = player1->AddComponent<CubeMesh>();
    cube->material = Material::Gems::Turquoise;

    GameObject* floor = testApp.CreateObject("Floor");
    transform = floor->AddComponent<Transform>();
    transform->position = vec3(0, 0, floorHeight);
    transform->scale = vec3(roomWidth, roomLength, 1);
    cube = floor->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* roof = testApp.CreateObject("Roof");
    transform = roof->AddComponent<Transform>();
    transform->position = vec3(0, 0, ceilingHeight);
    transform->scale = vec3(roomWidth, roomLength, 1);
    cube = roof->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* northWall = testApp.CreateObject("North Wall");
    transform = northWall->AddComponent<Transform>();
    transform->position = vec3(0, 0.5f * roomLength, wallCenter);
    transform->scale = vec3(roomWidth, wallThickness, roomHeight);
    cube = northWall->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* southWall = testApp.CreateObject("South Wall");
    transform = southWall->AddComponent<Transform>();
    transform->position = vec3(0, -0.5f * roomLength, wallCenter);
    transform->scale = vec3(roomWidth, wallThickness, roomHeight);
    cube = southWall->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* westWall = testApp.CreateObject("West Wall");
    transform = westWall->AddComponent<Transform>();
    transform->position = vec3(-0.5f * roomWidth, 0, wallCenter);
    transform->scale = vec3(wallThickness, roomLength, roomHeight);
    cube = westWall->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* eastWall = testApp.CreateObject("East Wall");
    transform = eastWall->AddComponent<Transform>();
    transform->position = vec3(0.5f * roomWidth, 0, wallCenter);
    transform->scale = vec3(wallThickness, roomLength, roomHeight);
    cube = eastWall->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* lightA = testApp.CreateObject("LightA");
    transform = lightA->AddComponent<Transform>();
    transform->position = vec3(0.25f * roomWidth, 0, wallCenter);
    PointLight* light = lightA->AddComponent<PointLight>();
    light->color = vec3(1, 0, 0);
    light->power = 5;
    IdleMovement* mover = lightA->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0, 0, roomHeight * 0.25f);
    mover->moveFrequency = vec3(0, 0, 0.10f);
    mover->basePosition = vec3(0.25f * roomWidth, 0, wallCenter);

    GameObject* lightB = testApp.CreateObject("LightB");
    transform = lightB->AddComponent<Transform>();
    transform->position = vec3(-0.25f * roomWidth, 0, wallCenter);
    light = lightB->AddComponent<PointLight>();
    light->power = 2;
    light->color = vec3(0, 1, 0);
    mover = lightB->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0, 0.3f * roomWidth, roomHeight * 0.25f);
    mover->moveFrequency = vec3(0, 0.05f, 0.11f);
    mover->basePosition = vec3(-0.25f * roomWidth, 0, wallCenter);

    GameObject* lightC = testApp.CreateObject("LightC");
    transform = lightC->AddComponent<Transform>();
    transform->position = vec3(0, 0.25f * roomLength, wallCenter);
    light = lightC->AddComponent<PointLight>();
    light->power = 7;
    light->color = vec3(0, 0, 1);
    mover = lightC->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0, 0, roomHeight * 0.25f);
    mover->moveFrequency = vec3(0, 0, 0.09f);
    mover->basePosition = vec3(0, 0.25f * roomLength, wallCenter);
    
    //transform->rotation = 4.0f;
    GameObject* cam = testApp.GetCamera();
    cam->GetComponent<Transform>()->position = vec3(0, 0, wallCenter);
    cam->AddComponent<FPSController>();

    testApp.RunGameLoop();

    return 0;
}