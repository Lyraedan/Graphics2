#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"
#include "PerlinRevised.h"
#include "SceneNodeTile.h"
#include "SceneNodeWater.h"
#include "SceneNodeTree.h"
#include "SceneNodeFish.h"
/*
	Author: Luke Rapkin
*/
#include <math.h>

class SceneNodeChunk : public SceneNodeMesh
{

public:
	SceneNodeChunk(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph);

	void AddQuad(SceneNodeTile* mesh, float chunkX, float chunkZ, int x, int z, float scl, int index, float heights[4]);

	float tileSize = 2.0f;
	int chunkSize = 16; //50

	std::vector<SceneNodeEntity*> entities;
	SceneNodeTile* ground = new SceneNodeTile(L"Ground");
	SceneNodeTile* water = new SceneNodeWater(L"Water");

	// Trees are glitchy but demonstrate collision - They fly away when touched by the camera
	bool hasBeenGenerated = false;
	float renderDistance = 3;

	float DistanceFrom(XMFLOAT3 src, XMFLOAT3 dest);

	void LevelCamera();

	/// <summary>
	/// Made Obsolete by CalculateHeight - Left in incase we decide to use it elsewhere
	/// </summary>
	float BarryCentric(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT2 pos);
private:
	float CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale);
	XMFLOAT3 CalculateNormal(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3);
	XMFLOAT3 Normalize(XMFLOAT3 point);
	XMFLOAT3 offset;
	SceneGraph* sceneGraph;

	// Spawn fish and trees, Setting this to false makes the chunks generate and the max speed
	bool spawnEntities = true;
	bool spawnFish = true;
	bool spawnTrees = true;
	float minHeight = 1; // 10 | 0.005
};
