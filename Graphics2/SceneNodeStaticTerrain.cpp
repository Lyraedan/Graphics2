#include "SceneNodeStaticTerrain.h"

bool SceneNodeStaticTerrain::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeStaticTerrain::Tick(XMMATRIX& completeTransform)
{

}

void SceneNodeStaticTerrain::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/terrain.obj", L"models/terrain.png");
}

void SceneNodeStaticTerrain::Shutdown()
{

}