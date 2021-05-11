#pragma once

#include "SceneNodeEntity.h"
#include "Graphics2.h"

class SceneNodeTree : public SceneNodeEntity
{

public:
	SceneNodeTree(wstring name) : SceneNodeEntity(name) {};

	bool Initialise() override;
	void Update(XMFLOAT4 camPosition);
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

private:
	bool doFly = false;
	float ytest = 0;
};
