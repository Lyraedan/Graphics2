#include "SceneNodeTree.h"

bool SceneNodeTree::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTree::Tick(XMMATRIX& completeTransform)
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	if (AABB::Intersects(DirectXFramework::GetDXFramework()->GetCamera()->bounds, bounds)) {
		XMFLOAT3 prev = DirectXFramework::GetDXFramework()->GetCamera()->prevPosition;
		DirectXFramework::GetDXFramework()->GetCamera()->SetCameraPosition(prev.x, prev.y, prev.z);
		doFly = true;
	}

	// Make the tree fly if the camera collides with it
	if (doFly) {
		ytest += 0.01f;
		SetWorldTransform(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(bounds.x, ytest, bounds.z));
		bounds.SetPosition(bounds.x, ytest, bounds.z);
	}
}

void SceneNodeTree::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/tree.obj", L"models/tree.png");
	bounds.SetSize(6, 12, 6);
}

void SceneNodeTree::Shutdown()
{

}

void SceneNodeTree::PlaceAt(XMFLOAT3 pos)
{
	bounds.SetPosition(pos.x, pos.y, pos.z);
	SetWorldTransform(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(pos.x, pos.y, pos.z));
}
