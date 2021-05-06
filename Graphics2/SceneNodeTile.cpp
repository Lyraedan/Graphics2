#include "SceneNodeTile.h"

bool SceneNodeTile::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTile::Shutdown()
{
}

void SceneNodeTile::Tick(XMMATRIX& completeTransform)
{
}

void SceneNodeTile::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	texture = L"grass.bmp";
}

// Ref: Newell's Method -> https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
XMFLOAT3 SceneNodeTile::CalculateNormals(XMFLOAT3 poly)
{
	XMFLOAT3 normal = poly;
	for (int i = 0; i < GetVertices() - 1; i++) {
		Vertex p0 = GetVertex(i);
		Vertex p1 = GetVertex(i + 1);
		normal.x += (p0.Position.y - p1.Position.y) * (p0.Position.z + p1.Position.z);
		normal.y += (p0.Position.z - p1.Position.z) * (p0.Position.x + p1.Position.x);
		normal.z += (p0.Position.x - p1.Position.x) * (p0.Position.y + p1.Position.y);
	}
	XMFLOAT3 normalised = normalise(normal);
	return normalised;
}