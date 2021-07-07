/*
	Author: Luke Rapkin
*/
#pragma once

#include "SceneNodeEntity.h"
#include "Graphics2.h"

class SceneNodeTree : public SceneNodeEntity
{

public:
	SceneNodeTree(wstring name) : SceneNodeEntity(name) {};

	bool Initialise() override;
	virtual void Update(XMFLOAT4 camPosition) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

private:
	bool doFly = false;
	float ytest = 0;
};
