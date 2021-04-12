#include "SceneNodeChunk.h"
#include "SceneNodeWater.h"
#include "SceneNodeTile.h";

bool SceneNodeChunk::Initialise()
{
	return true;
}

void SceneNodeChunk::Render()
{
}

void SceneNodeChunk::Shutdown()
{
}

void SceneNodeChunk::GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraphPointer sceneGraph) {
	UpdateHeight(terrainOffset.x, terrainOffset.z);
	int minHeight = 10;
	for (int z = 0; z < chunkSize - 1; z++) {
		for (int x = 0; x < chunkSize - 1; x++) {

			if (terrain[x][z] > minHeight) {
				//Todo swap to a single mesh instead of a bunch of tiles
				shared_ptr<SceneNodeTile> mesh = make_shared<SceneNodeTile>(L"Tile");
				int scl = 1;
				mesh->AddVertex(XMFLOAT3(-scl + x, terrain[x][z], -scl + z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(-scl + x, terrain[x][z + 1], scl + z), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, terrain[x + 1][z], -scl + z), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, terrain[x + 1][z + 1], scl + z), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

				sceneGraph->Add(mesh);
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(terrainOffset.x + -chunkSize + x * scl, terrainOffset.y, terrainOffset.z + -chunkSize + z * scl));
			}
			if (terrain[x][z] < minHeight + 2) {
				int scl = 1;
				float waterHeight = minHeight + 2;
				shared_ptr<SceneNodeWater> mesh = make_shared<SceneNodeWater>(L"Water");
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, -scl + z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, scl + z), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, -scl + z), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, scl + z), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

				sceneGraph->Add(mesh);
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(terrainOffset.x + -chunkSize + x * scl, terrainOffset.y + waterHeight, terrainOffset.z + -chunkSize + z * scl));
			}
		}
	}
}

/// <summary>
/// Generate a height map using perlin noise
/// </summary>
/// <param name="xOffset">- Our sampling X offset</param>
/// <param name="zOffset">- Our sampling Z offset</param>
void SceneNodeChunk::UpdateHeight(float xOffset, float zOffset)
{
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			float noise = PerlinNoise::perlin(xOffset, zOffset, xOffset / zOffset); // xOffset / zOffset
			float frequancy = 25.0;
			terrain[x][z] = (noise * frequancy);
			zOffset += 1.0 / chunkSize;
		}
		xOffset += 1.0 / chunkSize;
	}
}
