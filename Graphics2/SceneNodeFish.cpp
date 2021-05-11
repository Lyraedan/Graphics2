#include "SceneNodeFish.h"

bool SceneNodeFish::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeFish::Tick(XMMATRIX& completeTransform)
{
	SetWorldTransform(
		// Scale
		XMMatrixScaling(scale, scale, scale) *
		// Position
		XMMatrixTranslation(bounds.x, bounds.y, bounds.z) *
		// Rotation
		XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationY * 0.01f * XM_PI / 180.0f)
	);
	rotationY++;
}

void SceneNodeFish::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/fish.obj", L"textures/fish.png");
}

void SceneNodeFish::Shutdown()
{

}