#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"

class SceneNodeTree : public SceneNodeObjModel
{

public:
	SceneNodeTree(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	void PlaceAt(XMFLOAT3 pos);

	AABB bounds;

private:
	float scale = 1.0f;
	bool doFly = false;
	float ytest = 0;
};
