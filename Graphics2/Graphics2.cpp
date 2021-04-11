#include "Graphics2.h"
#include "SceneNodeTerrain.h"
#include "SceneNodePhysicsCube.h"
#include "SceneNodeWater.h"
#include "SceneNodeSkybox.h"
#include "SceneNodePlane.h"
#include "PerlinNoise.h"
#include <stdlib.h>
#include <time.h>

Graphics2 app;

constexpr long chunkSize = 50; //50
float terrain[chunkSize][chunkSize];

float flyX = 0;
float flyZ = 0;
float speed = 5.0f;

void Graphics2::CreateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	/*
	// Apply a seed to the generator
	unsigned int stime = static_cast<unsigned int>(time(NULL));
	srand(stime);

	// Generate our terrain
	UpdateHeight(rand(), rand());

	// Create our mesh and apply the generated heightmap to it
	GenerateTerrain(XMFLOAT3(0, 0, 0), sceneGraph);
	
	// Place other objects
	shared_ptr<SceneNodePhysicsCube> cube = make_shared<SceneNodePhysicsCube>(L"PhysicsCube");
	cube->SetHeightMap(terrain);
	cube->aabb.SetSize(5, 5, 5);
	cube->SetPosition(0, 100, 0);
	sceneGraph->Add(cube);
	*/
	
	// Skybox
	float skyScale = 50;
	shared_ptr<SceneNodeSkybox> sky = make_shared<SceneNodeSkybox>(L"Skybox");
	sceneGraph->Add(sky);
	sky->SetWorldTransform(XMMatrixScaling(skyScale, skyScale, skyScale) * XMMatrixTranslation(0, 0, 0));
	

	// Plane
	shared_ptr<SceneNodePlane> plane = make_shared<SceneNodePlane>(L"Plane");
	sceneGraph->Add(plane);
	plane->SetWorldTransform(XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixTranslation(0, 0, 0));


}

void Graphics2::UpdateSceneGraph()
{
	SceneGraphPointer sceneGraph = GetSceneGraph();

	// This is where you make any changes to the local world transformations to nodes
	// in the scene graph
	//sceneGraph->Update(GetViewTransformation() * GetProjectionTransformation());

	//flyX += speed;
	//flyZ += speed;
	//UpdateHeight(flyX, flyZ);

}

void Graphics2::GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraphPointer &sceneGraph) {
	int minHeight = 10;
	for (int z = 0; z < chunkSize - 1; z++) {
		for (int x = 0; x < chunkSize - 1; x++) {

			if (terrain[x][z] > minHeight) {
				shared_ptr<SceneNodeTerrain> mesh = make_shared<SceneNodeTerrain>(L"Terrain");
				int scl = 1;
				mesh->AddVertex(XMFLOAT3(-scl + x, terrain[x][z], -scl + z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(-scl + x, terrain[x][z + 1], scl + z), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, terrain[x + 1][z], -scl + z), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, terrain[x + 1][z + 1], scl + z), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

				sceneGraph->Add(mesh);
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(terrainOffset.x + -chunkSize + x * scl, terrainOffset.y, terrainOffset.z + -chunkSize + z * scl));
			}
			if (terrain[x][z] < minHeight + 2) {
				int scl = 1;
				float waterHeight = minHeight + 2;
				shared_ptr<SceneNodeWater> mesh = make_shared<SceneNodeWater>(L"Water");
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, -scl + z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, scl + z), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, -scl + z), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, scl + z), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

				sceneGraph->Add(mesh);
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(terrainOffset.x + -chunkSize + x * scl, terrainOffset.y + waterHeight, terrainOffset.z + -chunkSize + z * scl));
			}
		}
	}
}

/// <summary>
/// Generate a height map using perlin noise
/// </summary>
/// <param name="xOffset">- Our sampling X offset</param>
/// <param name="zOffset">- Our sampling Z offset</param>
void Graphics2::UpdateHeight(float xOffset, float zOffset)
{
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			float noise = PerlinNoise::perlin(xOffset, zOffset, xOffset / zOffset); // xOffset / zOffset
			float frequancy = 25.0;
			terrain[x][z] = (noise * frequancy);
			zOffset += 1.0 / chunkSize;
		}
		xOffset += 1.0 / chunkSize;
	}
}
