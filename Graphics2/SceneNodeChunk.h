#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"
#include "PerlinNoise.h"
#include "PerlinRevised.h"
#include "SceneNodeTile.h"
#include "SceneNodeWater.h"
#include "SceneNodeTree.h"
#include "SceneNodeBird.h"
#include <math.h>

class SceneNodeChunk : public SceneNodeMesh
{

public:
	SceneNodeChunk(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph);
	float terrain[16][16];

	void AddQuad(SceneNodeTile* mesh, float chunkX, float chunkZ, int x, int z, float scl, int index, float heights[4]);

	float tileSize = 2.0f;
	int chunkSize = 16; //50

	std::vector<SceneNodeMesh*> foilage;
	SceneNodeTile* ground = new SceneNodeTile(L"Ground");
	SceneNodeTile* water = new SceneNodeWater(L"Water");

	// Trees are glitchy but demonstrate collision - They fly away when touched by the camera
	bool hasBeenGenerated = false;
	float renderDistance = 3;

	float DistanceFrom(XMFLOAT3 src, XMFLOAT3 dest);

private:
	float CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale);
	XMFLOAT3 CalculateNormal(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3);
	XMFLOAT3 Normalize(XMFLOAT3 point);
	XMFLOAT3 offset;
	SceneGraph* sceneGraph;
	bool foilageAdded = false;
};
