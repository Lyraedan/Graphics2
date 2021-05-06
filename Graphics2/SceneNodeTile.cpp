#include "SceneNodeTile.h"

bool SceneNodeTile::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTile::Shutdown()
{
}

void SceneNodeTile::Tick(XMMATRIX& completeTransform)
{
}

void SceneNodeTile::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	texture = L"grass.bmp";
}