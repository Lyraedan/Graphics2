#include "SceneNodePlane.h"

bool SceneNodePlane::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodePlane::Tick(XMMATRIX& completeTransform)
{
	// TODO Clamp plane infront of camera
	//SetWorldTransform(XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(0, 50, 0));
	/*
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);

	float yaw = DirectXFramework::GetDXFramework()->GetCamera()->GetYaw();
	float pitch = DirectXFramework::GetDXFramework()->GetCamera()->GetPitch();
	float roll = DirectXFramework::GetDXFramework()->GetCamera()->GetRoll();
	SetWorldTransform(
		// Scale
		XMMatrixScaling(1, 1, 1) * 
		// Position
		XMMatrixTranslation(position.x, position.y - 5, position.z + 50) * 
		// Rotation
		XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), yaw * 1.0f * XM_PI / 180.0f) *
		XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), roll * 1.0f * XM_PI / 180.0f) *
		XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), pitch * 1.0f * XM_PI / 180.0f)
	);
	*/

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