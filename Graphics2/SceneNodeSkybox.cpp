#include "SceneNodeSkybox.h"

bool SceneNodeSkybox::Initialise()
{
	return SceneNodeMesh::Initialise();
}

float skyScl = 150;
float skyY = 0;
void SceneNodeSkybox::Tick(XMMATRIX& completeTransform)
{
	skyY++;
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	SetWorldTransform(XMMatrixScaling(skyScl, skyScl, skyScl) * XMMatrixTranslation(position.x, position.y, position.z + (skyScl / 2)));

}

void SceneNodeSkybox::SetupMesh() {
	BuildRendererState(D3D11_CULL_FRONT);
	LoadModel("models/skybox.obj", L"models/skybox.png");
}

void SceneNodeSkybox::Shutdown()
{

}