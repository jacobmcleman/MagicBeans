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
#include "DirectionalLight.h"
#include "SpotLight.h"
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
    cube->material = Material::Metals::Gold;

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
    transform->position = vec3(30, 0.3f * roomLength, wallCenter);
    transform->scale = vec3(3, 3, 3);
    PointLight* light = lightA->AddComponent<PointLight>();
    light->color = vec3(1.0f, 1.0f, 1.0f);
    light->power = 150;
    IdleMovement* mover = lightA->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0.2f * roomWidth, 1.3f * roomLength, roomHeight * 0.07f);
    mover->moveFrequency = vec3(0.03f, 0.04f, 0.10f);
    mover->basePosition = vec3(0, 0.9f * roomLength, wallCenter);
    //cube = lightA->AddComponent<CubeMesh>();
    //cube->material = Material::Plastics::Red;
    //
    GameObject* lightB = testApp.CreateObject("LightB");
    transform = lightB->AddComponent<Transform>();
    transform->position = vec3(0.25f * roomWidth, 0, wallCenter);
    transform->scale = vec3(3, 3, 3);
    light = lightB->AddComponent<PointLight>();
    light->power = 75;
    light->color = vec3(0.0f, 0.7f, 1.0f);
    mover = lightB->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0, 0.3f * roomLength, roomHeight * 0.25f);
    mover->moveFrequency = vec3(0, 0.05f, 0.11f);
    mover->basePosition = vec3(-0.25f * roomWidth, 0, wallCenter);
    //cube = lightB->AddComponent<CubeMesh>();
    //cube->material = Material::Plastics::Green;

    

    GameObject* lightC = testApp.CreateObject("LightC");
    transform = lightC->AddComponent<Transform>();
    transform->position = vec3(0, 0.25f * roomLength, wallCenter);
    transform->scale = vec3(3, 3, 3);
    light = lightC->AddComponent<PointLight>();
    light->power = 70;
    light->color = vec3(1.0f, 0, 0.7f);
    mover = lightC->AddComponent<IdleMovement>();
    mover->moveMagnitude = vec3(0.25f * roomWidth, 0.32f * roomHeight, roomHeight * 0.25f);
    mover->moveFrequency = vec3(0.06f, 0.025f, 0.09f);
    mover->basePosition = vec3(0, 0.25f * roomLength, wallCenter);
    //cube = lightC->AddComponent<CubeMesh>();
    //cube->material = Material::Plastics::Cyan;

    GameObject* floor2 = testApp.CreateObject("Floor");
    transform = floor2->AddComponent<Transform>();
    transform->position = vec3(0, roomLength * 1.5f, floorHeight);
    transform->scale = vec3(roomWidth, roomLength * 2, 1);
    cube = floor2->AddComponent<CubeMesh>();
    cube->material = roomMat;

    float curLinePos = 0;
    float materialSampleSize = 8;
    float materialSampleSpacing = materialSampleSize * 1.1f;
    float materialSampleWaveHeight = 2;

#define PLACE_MATERIAL_SAMPLE(mat, category) \
    GameObject* mat##Cube = testApp.CreateObject( #mat ); \
    transform = mat##Cube->AddComponent<Transform>(); \
    transform->position = vec3(roomWidth * 0.3f, roomLength * 0.8f + curLinePos, floorHeight + (materialSampleSize / 2)); \
    transform->scale = vec3(materialSampleSize, materialSampleSize, materialSampleSize); \
    curLinePos += materialSampleSpacing; \
    cube = mat##Cube->AddComponent<CubeMesh>(); \
    cube->material = Material::##category##::##mat 

    
    //////////////////// GEMS ///////////////////
    PLACE_MATERIAL_SAMPLE(Emerald, Gems);
    PLACE_MATERIAL_SAMPLE(Jade, Gems);
    PLACE_MATERIAL_SAMPLE(Obsidian, Gems);
    PLACE_MATERIAL_SAMPLE(Pearl, Gems);
    PLACE_MATERIAL_SAMPLE(Ruby, Gems);
    PLACE_MATERIAL_SAMPLE(Turquoise, Gems);
    ////////////////// METALS ///////////////////
    PLACE_MATERIAL_SAMPLE(Brass, Metals);
    PLACE_MATERIAL_SAMPLE(Bronze, Metals);
    PLACE_MATERIAL_SAMPLE(Chrome, Metals);
    PLACE_MATERIAL_SAMPLE(Copper, Metals);
    PLACE_MATERIAL_SAMPLE(Gold, Metals);
    PLACE_MATERIAL_SAMPLE(Silver, Metals);
    ///////////////// PLASTICS //////////////////
    PLACE_MATERIAL_SAMPLE(Black, Plastics);
    PLACE_MATERIAL_SAMPLE(Cyan, Plastics);
    PLACE_MATERIAL_SAMPLE(Green, Plastics);
    PLACE_MATERIAL_SAMPLE(Red, Plastics);
    PLACE_MATERIAL_SAMPLE(White, Plastics);
    PLACE_MATERIAL_SAMPLE(Yellow, Plastics);


#undef PLACE_MATERIAL_SAMPLE

    //GameObject* lightD = testApp.CreateObject("LightD");
    //transform = lightD->AddComponent<Transform>();
    //transform->position = vec3(0, roomLength, wallCenter + 5);
    //transform->scale = vec3(3, 3, 3);
    //light = lightD->AddComponent<PointLight>();
    //light->power = 140;
    //light->color = vec3(1, 1, 1);
    //mover = lightD->AddComponent<IdleMovement>();
    //mover->moveMagnitude = vec3(0, roomLength * 0.75f, roomHeight * 0.25f);
    //mover->moveFrequency = vec3(0.05f, -0.05f, 0);
    //mover->basePosition = vec3(0, 2 * roomLength, wallCenter + 5);
    //cube = lightD->AddComponent<CubeMesh>();
    //cube->material = Material::SelfIlluminated::WhiteLight;

    GameObject* floor3 = testApp.CreateObject("Floor");
    transform = floor3->AddComponent<Transform>();
    transform->position = vec3(roomWidth * 1.5f, roomLength * 1.5f, floorHeight);
    transform->scale = vec3(roomWidth * 2, roomLength * 2, 1);
    cube = floor3->AddComponent<CubeMesh>();
    cube->material = roomMat;

    GameObject* spotLight = testApp.CreateObject("Spotlight");
    transform = spotLight->AddComponent<Transform>();
    transform->position = vec3(roomWidth * 0.8f, roomLength * 0.8f, ceilingHeight * 2);
    transform->rotation = vec3(radians(-45.0f), 0, radians(-45.0f));
    //SpotLight* spotlight = spotLight->AddComponent<SpotLight>();
    //spotlight->color = vec3(1.0f, 1.0f, 0.8f);
    //spotlight->inner_cutoff = 0.9f;
    //spotlight->outer_cutoff = 0.89f;
    //spotlight->power = 50.0f;
    //cube = spotLight->AddComponent<CubeMesh>();
    //cube->material = Material::SelfIlluminated::WhiteLight;


    {
      GameObject* snowmanBodyA = testApp.CreateObject("Snowman Lower Body");
      transform = snowmanBodyA->AddComponent<Transform>();
      transform->position = vec3(roomWidth * 1.3f, roomLength * 1.3f, floorHeight + 5.5f);
      transform->rotation = vec3(0, 0, radians(-30.0f));
      transform->scale = vec3(10, 10, 10);
      cube = snowmanBodyA->AddComponent<CubeMesh>();
      cube->material = Material::Metals::Gold;

      GameObject* snowmanBodyB = testApp.CreateObject("Snowman Mid Body");
      transform = snowmanBodyB->AddComponent<Transform>();
      transform->position = vec3(roomWidth * 1.3f, roomLength * 1.3f, floorHeight + 13.0f);
      transform->rotation = vec3(0, 0, radians(-30.0f));
      transform->scale = vec3(7.5f, 7.5f, 7.5f);
      cube = snowmanBodyB->AddComponent<CubeMesh>();
      cube->material = Material::Metals::Gold;

      GameObject* snowmanBodyC = testApp.CreateObject("Snowman Top Body");
      transform = snowmanBodyC->AddComponent<Transform>();
      transform->position = vec3(roomWidth * 1.3f, roomLength * 1.3f, floorHeight + 18.0f);
      transform->rotation = vec3(0, 0, radians(-30.0f));
      transform->scale = vec3(5, 5, 5);
      cube = snowmanBodyC->AddComponent<CubeMesh>();
      cube->material = Material::Metals::Gold;
    }


    GameObject* sun = testApp.CreateObject("Sun");
    transform = sun->AddComponent<Transform>();
    transform->position = vec3(0, 0, 50);
    transform->rotation = vec3(radians(-60.0f), 0, radians(-60.0f));
    DirectionalLight* dirLight = sun->AddComponent<DirectionalLight>();
    dirLight->color = vec3(1, 1, 1);
    dirLight->shadow_distance = 200;
    
    //transform->rotation = 4.0f;
    GameObject* cam = testApp.GetCamera();
    cam->GetComponent<Transform>()->position = vec3(0, 0, wallCenter);
    cam->AddComponent<FPSController>();

    testApp.RunGameLoop();

    return 0;
}