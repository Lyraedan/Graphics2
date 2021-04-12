#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"
#include "ObjLoader.h"
#include "AABB.h"

class SceneNodePhysicsCube : public SceneNodeObjModel
{

public:
	SceneNodePhysicsCube(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	void setPosition(float x, float y, float z);

	void SetHeightMap(float heightmap[16][16]);

	AABB aabb;

	float xPos = 0;
	float yPos = 30;
	float zPos = 0;
	float terrain[50][50];

	float yRot = 0;
};
