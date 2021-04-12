#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"
#include "PerlinNoise.h"

class SceneNodeChunk : public SceneNodeMesh
{
	//Gerstner wave
public:
	SceneNodeChunk(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraphPointer sceneGraph);
	float terrain[50][50];

	int chunkSize = 50; //50
private:
	void UpdateHeight(float xOffset, float zOffset);

};
