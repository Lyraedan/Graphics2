#pragma once

#include "SceneNodeMesh.h"
#include "Graphics2.h"
#include "ObjLoader.h"

class SceneNodeObjModel : public SceneNodeMesh
{
public:
	SceneNodeObjModel(wstring name) : SceneNodeMesh(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;

	void LoadModel(string dir, const wchar_t* texture);

private:
	ObjLoader objLoader;
};
