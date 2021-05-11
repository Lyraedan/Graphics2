#include "Graphics2.h"
#include "SceneNodeTerrain.h"
#include "SceneNodeSkybox.h"
#include "SceneNodePlane.h"
#include <stdlib.h>
#include <time.h>

Graphics2 app;

void Graphics2::CreateSceneGraph()
{
	SceneGraph* sceneGraph = GetSceneGraph();

	GetCamera()->SetCameraPosition(0.0f, 30.0f, 0.0f);

	// Apply a seed to the generator
	unsigned int stime = static_cast<unsigned int>(time(NULL));
	srand(stime);

	SceneNodeTerrain* terrain = new SceneNodeTerrain(L"Terrain");
	terrain->SetSceneGraph(sceneGraph);
	sceneGraph->Add(terrain);

	SceneNodePlane* plane = new SceneNodePlane(L"Plane");
	sceneGraph->Add(plane);
	plane->SetWorldTransform(XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, 0, 0));

	SceneNodeSkybox* sky = new SceneNodeSkybox(L"Skybox");
	sceneGraph->Add(sky);
}

void Graphics2::UpdateSceneGraph()
{
	SceneGraph* sceneGraph = GetSceneGraph();
	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
}
