#pragma once
#include "DirectXFramework.h"

class Graphics2 : public DirectXFramework
{
public:
	void CreateSceneGraph();
	void UpdateSceneGraph();

	void GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraphPointer &sceneGraph);
	void UpdateHeight(float xOffset, float zOffset);
};

