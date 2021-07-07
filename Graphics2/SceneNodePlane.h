/*
	Author: Luke Rapkin
*/
#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"

class SceneNodePlane : public SceneNodeObjModel
{

public:
	SceneNodePlane(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

private:
	float scale = 1.0f;
	float rotationY;
	float xPos, zPos;
};
