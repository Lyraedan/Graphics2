#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"
#include "PerlinNoise.h"
#include "PerlinRevised.h"
#include "SceneNodeTile.h"
#include <math.h>

class SceneNodeChunk : public SceneNodeMesh
{
	//Gerstner wave
public:
	SceneNodeChunk(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph);
	float terrain[16][16];

	void AddQuad(SceneNodeTile* mesh, float chunkX, float chunkZ, int x, int z, float scl, int index, float heights[4]);

	int chunkSize = 16; //50
	std::vector<SceneNode*> entities;
private:
	void UpdateHeight(float xOffset, float zOffset);
	// Updated height calculation
	float CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale);


};
