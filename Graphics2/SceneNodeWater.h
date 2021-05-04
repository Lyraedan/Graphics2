#pragma once

#include "SceneNodeTile.h"
#include "Graphics2.h"

class SceneNodeWater : public SceneNodeTile
{
	//Gerstner wave
public:
	SceneNodeWater(wstring name) : SceneNodeTile(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;
	virtual void SetupMesh() override;
};