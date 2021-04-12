#include "SceneNodeTree.h"

bool SceneNodeTree::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTree::Tick(XMMATRIX& completeTransform)
{

}

void SceneNodeTree::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/tree.obj", L"models/tree.bmp");
}

void SceneNodeTree::Shutdown()
{

}