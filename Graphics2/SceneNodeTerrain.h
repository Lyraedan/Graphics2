#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "PerlinNoise.h"

class SceneNodeTerrain : public SceneNode
{
public:
	SceneNodeTerrain(wstring name) : SceneNode(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset);
	void UpdateHeight(float xOffset, float zOffset);
	void SetSceneGraph(SceneGraphPointer ptr);

	int chunkSize = 50; //50
	float terrain[50][50];

private:
	// making this & breaks the constructor
	SceneGraphPointer sceneGraph;
};
