#pragma once

#include "SceneNodeObjModel.h"
/*
	Author: Luke Rapkin
*/
#include "Graphics2.h"

class SceneNodeEntity : public SceneNodeObjModel
{

public:
	SceneNodeEntity(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Update(XMFLOAT4 camPosition);
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

	void PlaceAt(XMFLOAT3 pos);

	AABB bounds;
	float scale = 1.0f;
};
