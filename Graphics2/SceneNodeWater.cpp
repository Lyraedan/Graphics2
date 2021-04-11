#include "SceneNodeWater.h"

bool SceneNodeWater::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeWater::Shutdown()
{
}

void SceneNodeWater::SetupMesh()
{
	texture = L"water.bmp";
	AddIndice(0, 1, 2);
	AddIndice(2, 1, 3);
}
