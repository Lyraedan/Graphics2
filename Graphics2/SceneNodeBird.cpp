#include "SceneNodeBird.h"

bool SceneNodeBird::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeBird::Tick(XMMATRIX& completeTransform)
{
	//SetWorldTransform(XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(bounds.x, bounds.y, bounds.z));
	//bounds.SetPosition(bounds.x, bounds.y, bounds.z);
}

void SceneNodeBird::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/bird.obj", L"models/default.bmp");
	bounds.SetSize(scale * 12, scale * 12, scale * 12); // 6, 12, 6
}

void SceneNodeBird::Shutdown()
{

}

void SceneNodeBird::PlaceAt(XMFLOAT3 pos)
{
	bounds.SetPosition(pos.x, pos.y, pos.z);
	SetWorldTransform(XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(pos.x, pos.y, pos.z));
}