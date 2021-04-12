#include "SceneNodeTile.h"

bool SceneNodeTile::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTile::Shutdown()
{
}

void SceneNodeTile::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	texture = L"grass.bmp";
	AddIndice(0, 1, 2);
	AddIndice(2, 1, 3);
}
