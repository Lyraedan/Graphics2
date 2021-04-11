#include "SceneNodePlane.h"

bool SceneNodePlane::Initialise()
{
	return SceneNodeMesh::Initialise();
}


float planeY = 0;
void SceneNodePlane::Tick(XMMATRIX& completeTransform)
{
	planeY++;
	SetWorldTransform(XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixTranslation(0, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), planeY * 0.5f * XM_PI / 180.0f));

}

void SceneNodePlane::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	LoadModel("models/plane.obj", L"models/plane.bmp");
}

void SceneNodePlane::Shutdown()
{

}