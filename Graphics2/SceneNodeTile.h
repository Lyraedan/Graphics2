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
	virtual void SetupMesh() override;

	XMFLOAT3 CalculateNormals(XMFLOAT3 poly);
};
