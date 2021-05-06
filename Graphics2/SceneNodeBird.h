#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"

class SceneNodeBird : public SceneNodeObjModel
{

public:
	SceneNodeBird(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	void PlaceAt(XMFLOAT3 pos);

	AABB bounds;

private:
	float scale = 1.0f;
};