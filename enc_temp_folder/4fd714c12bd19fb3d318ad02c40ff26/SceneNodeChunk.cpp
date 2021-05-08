#include "SceneNodeChunk.h"

bool SceneNodeChunk::Initialise()
{
	return true;
}

void SceneNodeChunk::Render()
{
	if (!hasBeenGenerated) return;

	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	/*
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			if (position.y <= terrain[x][z]) {
				// Old method for preventing the camera from going under the terrain
				//DirectXFramework::GetDXFramework()->GetCamera()->SetCameraPosition(position.x, terrain[x][z] + 0.5f, position.z);
			}
		}
	}
	*/

	float distanceFromCamera = DistanceFrom(offset, XMFLOAT3(position.x, position.y, position.z));
	ground->doRender = distanceFromCamera < chunkSize * renderDistance;
}

void SceneNodeChunk::Shutdown()
{
	//delete ground;
	//delete water;
	//delete tree;
}

void SceneNodeChunk::GenerateTerrain(XMFLOAT3 terrainOffset, SceneGraph* sceneGraph) {
	this->offset = terrainOffset;
	float chunkX = (terrainOffset.x * chunkSize);
	float chunkZ = (terrainOffset.z * chunkSize);

	float minHeight = 1; // 10 | 0.005

	int groundIndex = 0;
	int waterIndex = 0;
	for (int z = 0; z <= chunkSize - 1; z++) {
		for (int x = 0; x <= chunkSize - 1; x++) {

			float tr = CalculateHeight(chunkX, chunkZ, x, z + 1, tileSize);
			float tl = CalculateHeight(chunkX, chunkZ, x, z, tileSize);
			float bl = CalculateHeight(chunkX, chunkZ, x + 1, z, tileSize);
			float br = CalculateHeight(chunkX, chunkZ, x + 1, z + 1, tileSize);
			float avg = (tr + tl + bl + br) / 4;

			AddQuad(ground, chunkX, chunkZ, x, z, tileSize, groundIndex, new float[4]{ tr, tl, bl, br });
			groundIndex += 4;

			float waterHeight = minHeight - 2;
			AddQuad(water, chunkX, chunkZ, x, z, tileSize, waterIndex, new float[4]{ waterHeight, waterHeight, waterHeight, waterHeight });
			waterIndex += 4;

			if (avg > waterHeight) {
				bool spawn = rand() % 1000 == 0;
				if (spawn && generateTrees) {
					SceneNodeTree* test = new SceneNodeTree(L"Test");
					test->PlaceAt(XMFLOAT3(chunkX + x, avg, chunkZ + z));
					sceneGraph->Add(test);
				}
			}
		}
	}

	entities.push_back(ground);
	entities.push_back(water);

	for (auto c : entities) {
		c->Initialise();
	}
	hasBeenGenerated = true;
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
	XMFLOAT3 n1 = CalculateNormal(topLeft, bottomLeft, bottomRight);
	XMFLOAT3 n2 = n1;
	XMFLOAT3 n3 = n1;
	XMFLOAT3 n4 = CalculateNormal(topLeft, bottomRight, topRight);

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

float SceneNodeChunk::DistanceFrom(XMFLOAT3 src, XMFLOAT3 dest)
{
	return sqrt(pow(dest.x - src.x, 2) +
				pow(dest.y - src.y, 2) +
				pow(dest.z - src.z, 2) * 1.0f);
}

/// <summary>
/// New heightmap calculation
/// </summary>
/// <param name="chunkX">Chunk offset X</param>
/// <param name="chunkZ">Chunk offset Z</param>
/// <param name="x">Tile index X</param>
/// <param name="z">Tile index Z</param>
/// <param name="tileScale">Tile size</param>
/// <returns>Noise value at that given coordinate</returns>
float SceneNodeChunk::CalculateHeight(float chunkX, float chunkZ, float x, float z, float tileScale)
{
	float offset = 5000.0f;
	float perlinX = ((offset + chunkX + x) * tileScale) / chunkSize;
	float perlinZ = ((offset + chunkZ + z) * tileScale) / chunkSize;

	float noise = PerlinRevised::perlin(perlinX, perlinZ);
	float frequancy = 8;
	return noise * frequancy;
}

XMFLOAT3 SceneNodeChunk::CalculateNormal(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3)
{
	XMFLOAT3 a = XMFLOAT3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	XMFLOAT3 b = XMFLOAT3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);


	return Normalize(XMFLOAT3(
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x));
}

XMFLOAT3 SceneNodeChunk::Normalize(XMFLOAT3 point)
{
	double length = sqrt(point.x * point.x + point.y * point.y + point.z * point.z);

	return XMFLOAT3(
		point.x / length,
		point.y / length,
		point.z / length);
}
