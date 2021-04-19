#include "SceneNodeTile.h"

bool SceneNodeTile::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeTile::Shutdown()
{
}

void SceneNodeTile::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	texture = L"grass.bmp";
	AddIndice(0, 1, 2);
	AddIndice(2, 1, 3);
}

// Ref: Newell's Method -> https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
XMFLOAT3 SceneNodeTile::CalculateNormals(XMFLOAT3 poly)
{
	XMFLOAT3 normal = poly;
	for (int i = 0; i < GetVertices(); i++) {
		Vertex current = GetVertex(i);
		Vertex next = GetVertex((i + 1) % GetVertices());
		normal.x = normal.x + ((current.Position.y - next.Position.y) * (current.Position.z + next.Position.z));
		normal.y = normal.y + ((current.Position.z - next.Position.z) * (current.Position.x + next.Position.z));
		normal.z = normal.z + ((current.Position.x - next.Position.x) * (current.Position.y + next.Position.y));
	}
	return normalise(normal);
}
