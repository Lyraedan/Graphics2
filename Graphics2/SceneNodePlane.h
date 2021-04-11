#pragma once

#include "SceneNodeObjModel.h"
#include "Graphics2.h"
#include "WICTextureLoader.h"
#include "ObjLoader.h"
#include <list>

class SceneNodePlane : public SceneNodeObjModel
{

public:
	SceneNodePlane(wstring name) : SceneNodeObjModel(name) {};

	bool Initialise() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;
	virtual void Shutdown() override;

private:
	ObjLoader objLoader;
};
