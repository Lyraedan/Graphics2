#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"
#include "ObjLoader.h"

class SceneNodeSkybox : public SceneNodeObjModel
{

public:
	SceneNodeSkybox(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	float skyScl = 150; // 150
};
