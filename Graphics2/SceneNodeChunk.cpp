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
				//DirectXFramework::GetDXFramework()->GetCamera()->SetCameraPosition(position.x, terrain[x][z] + 0.5f, position.z);
			}
		}
	}
}

void SceneNodeChunk::Shutdown()
{
}

void SceneNodeChunk::GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph) {
	// Todo cleanup these equations
	float chunkX = terrainOffset.x * chunkSize;
	float chunkZ = terrainOffset.z * chunkSize;
	SceneNodeTile* mesh = new SceneNodeTile(L"Tile");

	UpdateHeight(chunkX, chunkZ);
	int scl = 1;
	float minHeight = 0.05f;
	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			AddQuad(mesh, x, z, scl, new float[4]{ 0, 0, 0, 0 });
		}
	}
	entities.push_back(mesh);

	SceneNodeTree* tree = new SceneNodeTree(L"Tree");
	XMFLOAT3 pos = XMFLOAT3(10, 0, 10);
	tree->PlaceAt(pos);
	entities.push_back(tree);
	for (int z = 0; z < chunkSize - 1; z++) {
		for (int x = 0; x < chunkSize - 1; x++) {
			if (terrain[x][z] > minHeight) {
				/*AddQuad(mesh, x, z, scl, new float[4] { terrain[x][z + 1],
														terrain[x][z],
														terrain[x + 1][z],
														terrain[x + 1][z + 1] },
														);
				entities.push_back(mesh);
				*/
				//Spawn trees
				/*
				bool spawnTree = std::rand() % chunkSize == 0;
				if (spawnTree) {
					SceneNodeTree* tree = new SceneNodeTree(L"Tree");
					//sceneGraph->Add(tree);
					//tree->Initialise();
					XMFLOAT3 pos = XMFLOAT3(chunkX + (x * 2), terrain[x][z] + 0.25f, chunkZ + (z * 2));
					tree->PlaceAt(pos);
					entities.push_back(tree);
				}
				*/

			}
			/*
			if (terrain[x][z] < minHeight + 2) {
				float waterHeight = minHeight + 2;
				SceneNodeWater* mesh = new SceneNodeWater(L"Water");
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, -scl + z), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(-scl + x, 0, scl + z), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, -scl + z), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
				mesh->AddVertex(XMFLOAT3(scl + x, 0, scl + z), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

				entities.push_back(mesh);
				mesh->SetWorldTransform(XMMatrixScaling(scl, scl, scl) * XMMatrixTranslation(chunkX + (x * scl), terrainOffset.y + waterHeight, chunkZ + (z * scl)));
			}
			*/
		}
	}


	int done = 0;
	for (auto c : entities) {
		c->Initialise();
		//sceneGraph->Add(c); // This is causing the crash
	}
}

void SceneNodeChunk::AddQuad(SceneNodeTile* mesh, int x, int z, float scl, float heights[4])
{
	XMFLOAT3 topLeft = XMFLOAT3(x * scl, heights[0], (z + 1) * scl);
	XMFLOAT3 bottomLeft = XMFLOAT3(x * scl, heights[1], z * scl);
	XMFLOAT3 bottomRight = XMFLOAT3((x + 1) * scl, heights[2], z * scl);
	XMFLOAT3 topRight = XMFLOAT3((x + 1) * scl, heights[3], (z + 1) * scl);
	// Normals
	XMFLOAT3 n1 = mesh->CalculateNormals(topLeft);
	XMFLOAT3 n2 = mesh->CalculateNormals(bottomLeft);
	XMFLOAT3 n3 = mesh->CalculateNormals(bottomRight);
	XMFLOAT3 n4 = mesh->CalculateNormals(topRight);

	// Textures
	XMFLOAT2 uvs[] = {
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(1.0f, 1.0f),
		XMFLOAT2(1.0f, 0.0f)
		
	};

	mesh->AddVertex(bottomLeft, n2, uvs[0]);
	mesh->AddVertex(topLeft, n1, uvs[1]);
	mesh->AddVertex(topRight, n4, uvs[2]);
	mesh->AddVertex(bottomRight, n3, uvs[3]);

	int index = (z * chunkSize) + x;
	mesh->AddIndice(index, index + 1, index + 2);
	mesh->AddIndice(index, index + 2, index + 3);
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
			zOffset += 1.0 / chunkSize; // todo change to give different variaty
		}
		xOffset += 1.0f / chunkSize; // todo change to give different variaty:lol
	}
}

float SceneNodeChunk::CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale)
{
	float perlinX = (chunkX + (x * tileScale)) / chunkSize;
	float perlinZ = (chunkZ + (z * tileScale)) / chunkSize;
	float noise = PerlinNoise::perlin(abs(perlinX), abs(perlinZ), 1);
	float frequancy = 25.0f;
	return noise * frequancy;
}
