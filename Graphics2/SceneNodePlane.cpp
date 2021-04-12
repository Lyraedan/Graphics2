#include "SceneNodePlane.h"

bool SceneNodePlane::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodePlane::Tick(XMMATRIX& completeTransform)
{
	SetWorldTransform(XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, 50, 0));

}

void SceneNodePlane::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	LoadModel("models/plane.obj", L"models/plane.bmp");
}

void SceneNodePlane::Shutdown()
{

}