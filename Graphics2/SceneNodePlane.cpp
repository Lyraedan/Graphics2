/*
	Author: Luke Rapkin
*/
#include "SceneNodePlane.h"

bool SceneNodePlane::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodePlane::Tick(XMMATRIX& completeTransform)
{
	SetWorldTransform(
		// Scale
		XMMatrixScaling(scale, scale, scale) *
		// Position
		XMMatrixTranslation(xPos, 50, zPos) *
		// Rotation
		XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationY * 0.1f * XM_PI / 180.0f)
	);
	rotationY += 0.1f;
	zPos++;

}

void SceneNodePlane::SetupMesh() {
	BuildRendererState(D3D11_CULL_BACK);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/plane.obj", L"textures/plane.bmp");
}

void SceneNodePlane::Shutdown()
{

}