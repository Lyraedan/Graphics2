#include "SceneNodePlane.h"

bool SceneNodePlane::Initialise()
{
	return SceneNodeMesh::Initialise();
}


float y = 0;
void SceneNodePlane::Tick(XMMATRIX& completeTransform)
{
	y++;
	SetWorldTransform(XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixTranslation(0, 0, 0) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), y * 0.5f * XM_PI / 180.0f));

}

void SceneNodePlane::SetupMesh() {
	// Load our plane model into memory
	BuildRendererState(D3D11_CULL_BACK);
	objLoader.LoadModel("models/plane.obj");
	texture = L"models/plane.bmp";

	for(ObjLoader::Face face : objLoader.faces) {
		for (XMFLOAT3 v : face.faceData) {
			XMFLOAT3 verts = XMFLOAT3(objLoader.verts.at((int) v.x - 1));
			XMFLOAT2 uv = XMFLOAT2(objLoader.uvs.at((int) v.y - 1));
			XMFLOAT3 normals = XMFLOAT3(objLoader.normals.at((int) v.z - 1));
			AddVertex(verts, normals, uv);
		}

		for (XMFLOAT3 tris : face.tris) {
			AddIndice(tris.x, tris.y, tris.z);
		}
	}
}

void SceneNodePlane::Shutdown()
{

}