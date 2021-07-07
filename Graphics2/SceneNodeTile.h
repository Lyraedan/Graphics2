/*
	Author: Luke Rapkin
*/
#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"

class SceneNodeTile : public SceneNodeMesh
{
	//Gerstner wave
public:
	SceneNodeTile(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
};
