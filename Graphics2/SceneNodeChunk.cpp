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
	float chunkX = terrainOffset.x * chunkSize;
	float chunkZ = terrainOffset.z * chunkSize;
	SceneNodeTile* ground = new SceneNodeTile(L"Tile");
	SceneNodeTile* water = new SceneNodeWater(L"Water");

	//UpdateHeight(chunkX, chunkZ);
	int scl = 2;
	float minHeight = 1; // 10 | 0.005

	int groundIndex = 0;
	int waterIndex = 0;
	for (int z = 0; z < chunkSize - 1; z++) {
		for (int x = 0; x < chunkSize - 1; x++) {

			float tr = CalculateHeight(chunkX, chunkZ, x, z + 1, scl);
			float tl = CalculateHeight(chunkX, chunkZ, x, z, scl);
			float bl = CalculateHeight(chunkX, chunkZ, x + 1, z, scl);
			float br = CalculateHeight(chunkX, chunkZ, x + 1, z + 1, scl);
			float avg = (tr + tl + bl + br) / 4;

			terrain[x][z + 1] = tr;
			terrain[x][z] = tl;
			terrain[x + 1][z] = bl;
			terrain[x + 1][z + 1] = br;

			if (avg > minHeight) {
				AddQuad(ground, chunkX, chunkZ, x, z, scl, groundIndex, new float[4]{ terrain[x][z + 1],
																					  terrain[x][z],
																					  terrain[x + 1][z],
																					  terrain[x + 1][z + 1] });
				groundIndex += 4;
				//Spawn trees
				bool spawnTree = std::rand() % chunkSize == 0;
				if (spawnTree) {
					if (x >= 10 && z >= 10) {
						SceneNodeTree* tree = new SceneNodeTree(L"Tree");
						XMFLOAT3 pos = XMFLOAT3(chunkX + x, terrain[x][z] + 0.2f, chunkZ + z);
						tree->PlaceAt(pos);
						entities.push_back(tree);
					}
				}
			}
			else {
				float waterHeight = minHeight;
				AddQuad(water, chunkX, chunkZ, x, z, scl, waterIndex, new float[4]{ waterHeight, waterHeight, waterHeight, waterHeight });
				waterIndex += 4;
			}
		}

		entities.push_back(ground);
		entities.push_back(water);
	}


	int done = 0;
	for (auto c : entities) {
		c->Initialise();
		//sceneGraph->Add(c); // This is causing the crash
	}
}

void SceneNodeChunk::AddQuad(SceneNodeTile* mesh, float chunkX, float chunkZ, int x, int z, float scl, int index, float heights[4])
{
	x += chunkX;
	z += chunkZ;
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

	//int index = (z * chunkSize) + x;
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
	//float noise = PerlinNoise::perlin(abs(perlinX), abs(perlinZ), 1);
	float noise = PerlinRevised::perlin(perlinX, perlinZ);
	float frequancy = 8;
	return noise * frequancy;
}
