#include "SceneNodePhysicsCube.h"

bool SceneNodePhysicsCube::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodePhysicsCube::Tick(XMMATRIX& completeTransform)
{
	int x = (int)(floor(xPos + zPos / 5)); //(xPos * 50) / 5; // / 50 // floor(xPos * 50) / 50
	int z = (int)(floor(zPos + xPos / 5)); //(zPos * 50) / 5; // / 50

	// Disable falling if passed a threashold
	/*
	if (yPos > -10) {
		if (yPos > terrain[x][z] + 5) {
			float gravity = 0.5f;
			yPos -= gravity;
		}
		else {
			yPos = terrain[x][z] + 5;
			xPos += 0.1f;
		}
	}

	*/
	if (yPos > terrain[x][z] + 1) {
		float gravity = 0.5f;
		yPos -= gravity;
	}
	else {
		yPos = terrain[x][z] + 1;
		xPos += 0.1f;
	}

	yRot++;
	SetWorldTransform(XMMatrixScaling(5, 5, 5) * XMMatrixTranslation(xPos, yPos, zPos)/* * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), yRot * 0.5f * XM_PI / 180.0f)*/);

}

void SceneNodePhysicsCube::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	LoadModel("models/block.obj", L"models/block.png");
}

void SceneNodePhysicsCube::Shutdown()
{

}

void SceneNodePhysicsCube::setPosition(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void SceneNodePhysicsCube::SetHeightMap(float heightMap[16][16])
{
	// I hate this :)
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			terrain[x][z] = heightMap[x][z];
		}
	}
}