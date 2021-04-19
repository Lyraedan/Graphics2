#include "SceneNodeChunk.h"
#include "SceneNodeWater.h"
#include "SceneNodeTree.h"
#include "SceneNodeTile.h"

bool SceneNodeChunk::Initialise()
{
	return true;
}

void SceneNodeChunk::Render()
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			if (position.y <= terrain[x][z]) {
				DirectXFramework::GetDXFramework()->GetCamera()->SetCameraPosition(position.x, terrain[x][z] + 0.5f, position.z);
			}
		}
	}
}

void SceneNodeChunk::Shutdown()
{
}

void SceneNodeChunk::GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph) {
	float chunkX = terrainOffset.x * chunkSize + (chunkSize + (chunkSize / 2));
	float chunkZ = terrainOffset.z * chunkSize + (chunkSize + (chunkSize / 2));
	//shared_ptr<SceneNodeTile> mesh = make_shared<SceneNodeTile>(L"Tile");

	//UpdateHeight(chunkX, chunkZ);
	float minHeight = 0.05f;
	for (int z = 0; z < chunkSize - 1; z++) {
		for (int x = 0; x < chunkSize - 1; x++) {
			int scl = 1;
			terrain[x][z] = CalculateHeight(chunkX, chunkZ, x, z, scl);
			terrain[x][z + 1] = CalculateHeight(chunkX, chunkZ, x, z + 1, scl);
			terrain[x + 1][z] = CalculateHeight(chunkX, chunkZ, x + 1, z, scl);
			terrain[x + 1][z + 1] = CalculateHeight(chunkX, chunkZ, x + 1, z + 1, scl);

			//if (terrain[x][z] > minHeight) {
				//Todo swap to a single mesh instead of a bunch of tiles
				SceneNodeTile* mesh = new SceneNodeTile(L"Tile");
				// Vertices
				XMFLOAT3 v1 = XMFLOAT3(-scl + x, terrain[x][z], -scl + z);
				XMFLOAT3 v2 = XMFLOAT3(-scl + x, terrain[x][z + 1], scl + z);
				XMFLOAT3 v3 = XMFLOAT3(scl + x, terrain[x + 1][z], -scl + z);
				XMFLOAT3 v4 = XMFLOAT3(scl + x, terrain[x + 1][z + 1], scl + z);

				// Normals
				XMFLOAT3 n1 = mesh->CalculateNormals(v1);
				XMFLOAT3 n2 = mesh->CalculateNormals(v2);
				XMFLOAT3 n3 = mesh->CalculateNormals(v3);
				XMFLOAT3 n4 = mesh->CalculateNormals(v4);

				mesh->AddVertex(v1, n1, XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(v2, n2, XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(v3, n3, XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(v4, n4, XMFLOAT2(1.0f, 1.0f));

				
				/*
				// Generate indices
				if (z < chunkSize - 1 && x < chunkSize - 1) {
					int leftTop = x * scl + z;
					int leftBottom = (x + 1) * scl + z;
					int rightBottom = (x + 1) * scl + z + 1;
					int rightTop = x * scl + z + 1;
					AddIndice(leftTop, rightTop, leftBottom);
					AddIndice(rightBottom, leftBottom, rightTop);
				}
				*/

				sceneGraph->Add(mesh);
				mesh->Initialise();
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(chunkX + (x * scl), terrainOffset.y, chunkZ + (z * scl)));

				//Spawn trees
				bool spawnTree = std::rand() % chunkSize == 0;
				if (spawnTree) {
					SceneNodeTree* tree = new SceneNodeTree(L"Tree");
					sceneGraph->Add(tree);
					tree->Initialise();
					tree->SetWorldTransform(XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(chunkX + (x * 2), terrain[x][z] + 0.25f, chunkZ + (z * 2)));

				}

			/*
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
				mesh->Initialise();
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(chunkX + (x * scl), terrainOffset.y + waterHeight, chunkZ + (z * scl)));
			}
			*/
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
			float noise = PerlinNoise::perlin(abs(xOffset), abs(zOffset), abs(xOffset / zOffset)); // xOffset / zOffset
			float frequancy = 25.0;
			terrain[x][z] = (noise * frequancy);
			zOffset += 1.0f / chunkSize; // todo change to give different variaty
		}
		xOffset += 1.0f / chunkSize; // todo change to give different variaty:lol
	}
}

float SceneNodeChunk::CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale)
{
	float perlinX = (chunkX + (x * tileScale)) / chunkSize;
	float perlinZ = (chunkZ + (z * tileScale)) / chunkSize;
	float noise = PerlinNoise::perlin(abs(perlinX), abs(perlinZ), 1);
	float frequancy = 1;
	return noise * frequancy;
}
