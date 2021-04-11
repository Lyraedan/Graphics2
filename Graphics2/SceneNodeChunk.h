#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"

class SceneNodeChunk : public SceneNodeMesh
{
	//Gerstner wave
public:
	SceneNodeChunk(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;
	virtual void SetupMesh() override;
};
