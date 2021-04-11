#include "SceneNodeChunk.h"

bool SceneNodeChunk::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeChunk::Shutdown()
{
}

void SceneNodeChunk::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	texture = L"grass.bmp";
	AddIndice(0, 1, 2);
	AddIndice(2, 1, 3);
}
