#include "SceneNodeTree.h"

bool SceneNodeTree::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTree::Update(XMFLOAT4 camPosition)
{
	if (AABB::Intersects(DirectXFramework::GetDXFramework()->GetCamera()->bounds, bounds)) {
		XMFLOAT3 prev = DirectXFramework::GetDXFramework()->GetCamera()->prevPosition;
		DirectXFramework::GetDXFramework()->GetCamera()->SetCameraPosition(prev.x, prev.y, prev.z);
		doFly = true;
	}

	// Make the tree fly if the camera collides with it
	if (doFly) {
		ytest += 0.001f;
		SetWorldTransform(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(bounds.x, bounds.y + ytest, bounds.z));
		bounds.SetPosition(bounds.x, bounds.y + ytest, bounds.z);
	}
}

void SceneNodeTree::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/tree2.obj", L"models/tree2.png");
	bounds.SetSize(scale * 12, scale * 24, scale * 12); // 6, 12, 6
}

void SceneNodeTree::Shutdown()
{

}

void SceneNodeTree::PlaceAt(XMFLOAT3 pos)
{
	bounds.SetPosition(pos.x, pos.y, pos.z);
	SetWorldTransform(XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(pos.x, pos.y, pos.z));
}
