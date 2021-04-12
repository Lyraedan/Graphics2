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
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/plane.obj", L"models/plane.bmp");
}

void SceneNodePlane::Shutdown()
{

}