#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"

class SceneNodeTree : public SceneNodeObjModel
{

public:
	SceneNodeTree(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	void Update(XMFLOAT4 camPosition);
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	void PlaceAt(XMFLOAT3 pos);

	AABB bounds;
	bool active = false;

private:
	float scale = 0.5f;
	bool doFly = false;
	float ytest = 0;
};
