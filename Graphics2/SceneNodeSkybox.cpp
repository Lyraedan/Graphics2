#include "SceneNodeSkybox.h"

bool SceneNodeSkybox::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeSkybox::Tick(XMMATRIX& completeTransform)
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	SetWorldTransform(XMMatrixScaling(skyScl, skyScl, skyScl) * XMMatrixTranslation(position.x, position.y, position.z + (skyScl / 2)));

}

void SceneNodeSkybox::SetupMesh() {
	BuildRendererState(D3D11_CULL_FRONT);
	ambientColour = XMFLOAT4(1, 1, 1, 1);
	lightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel("models/skybox.obj", L"models/skybox.png");
}

void SceneNodeSkybox::Shutdown()
{

}