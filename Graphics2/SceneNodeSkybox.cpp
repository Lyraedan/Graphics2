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
	SetWorldTransform(XMMatrixScaling(skyScl, skyScl, skyScl) * XMMatrixTranslation(0, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), skyY * 0.5f * XM_PI / 180.0f));

}

void SceneNodeSkybox::SetupMesh() {
	BuildRendererState(D3D11_CULL_FRONT);
	LoadModel("models/skybox.obj", L"models/skybox.png");
}

void SceneNodeSkybox::Shutdown()
{

}