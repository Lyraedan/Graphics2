#include "Graphics2.h"
#include "SceneNodeTerrain.h"
#include "SceneNodePhysicsCube.h"
#include "SceneNodeSkybox.h"
#include "SceneNodePlane.h"
#include <stdlib.h>
#include <time.h>

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	GetCamera()->SetCameraPosition(0.0f, 20.0f, -100.0f);

	// Skybox
	shared_ptr<SceneNodeSkybox> sky = make_shared<SceneNodeSkybox>(L"Skybox");
	sceneGraph->Add(sky);

	// Apply a seed to the generator
	unsigned int stime = static_cast<unsigned int>(time(NULL));
	srand(stime);

	/*
	shared_ptr<SceneNodeTerrain> terrain = make_shared<SceneNodeTerrain>(L"Terrain");
	terrain->SetSceneGraph(sceneGraph);
	terrain->GenerateTerrain(XMFLOAT3(0, 0, 0));
	sceneGraph->Add(terrain);

	// Place other objects
	shared_ptr<SceneNodePhysicsCube> cube = make_shared<SceneNodePhysicsCube>(L"PhysicsCube");
	cube->SetHeightMap(terrain->terrain);
	cube->aabb.SetSize(5, 5, 5);
	cube->setPosition(0, 30, 0);
	sceneGraph->Add(cube);
	cube->SetWorldTransform(XMMatrixScaling(5, 5, 5) * XMMatrixTranslation(0, 30, 0));
	*/

	// Plane
	shared_ptr<SceneNodePlane> plane = make_shared<SceneNodePlane>(L"Plane");
	sceneGraph->Add(plane);
	plane->SetWorldTransform(XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixTranslation(0, 0, 0));

}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	if (GetAsyncKeyState(VK_UP) < 0) {
		GetCamera()->SetForwardBack(1);
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0) {
		GetCamera()->SetForwardBack(-1);
	}
	
	if (GetAsyncKeyState(VK_LEFT) < 0) {
		GetCamera()->SetLeftRight(-1);
	} else if (GetAsyncKeyState(VK_RIGHT) < 0) {
		GetCamera()->SetLeftRight(1);
	}

	if (GetAsyncKeyState(VK_NUMPAD8) < 0) {
		GetCamera()->SetPitch(1);
	}
	else if (GetAsyncKeyState(VK_NUMPAD2) < 0) {
		GetCamera()->SetPitch(-1);
	}
	
	if (GetAsyncKeyState(VK_NUMPAD4) < 0) {
		GetCamera()->SetRoll(-1);
	} else if (GetAsyncKeyState(VK_NUMPAD6) < 0) {
		GetCamera()->SetRoll(1);
	}
	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
	//sceneGraph->Update(GetViewTransformation() * GetProjectionTransformation());
}
