#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"

class SceneNodeWater : public SceneNodeMesh
{
	//Gerstner wave
public:
	SceneNodeWater(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;
	virtual void SetupMesh() override;
};