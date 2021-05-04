#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"

class SceneNodeStaticTerrain : public SceneNodeObjModel
{

public:
	SceneNodeStaticTerrain(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;
};
