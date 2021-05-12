/*
	Author: Luke Rapkin
*/
#include "SceneNodeObjModel.h"

bool SceneNodeObjModel::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeObjModel::Shutdown()
{

}

void SceneNodeObjModel::LoadModel(string dir, const wchar_t* tex)
{
	objLoader.LoadModel(dir);
	texture = tex;

	for (ObjLoader::Face face : objLoader.faces) {
		for (XMFLOAT3 v : face.faceData) {
			XMFLOAT3 verts = XMFLOAT3(objLoader.verts.at((int)v.x - 1));
			XMFLOAT2 uv = XMFLOAT2(objLoader.uvs.at((int)v.y - 1));
			XMFLOAT3 normals = XMFLOAT3(objLoader.normals.at((int)v.z - 1));
			AddVertex(verts, normals, uv);
		}

		for (XMFLOAT3 tris : face.tris) {
			AddIndice(tris.x, tris.y, tris.z);
		}
	}
}
