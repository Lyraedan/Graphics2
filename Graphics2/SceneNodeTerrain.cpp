#include "SceneNodeTerrain.h"

bool SceneNodeTerrain::Initialise()
{
	//For demoing
	if (!generateDynamically) {
		int numberOfChunks = 5;
		//if number of chunks = 3 -> Generate 3 x 3 chunks = 9 
		for (int x = 0; x < numberOfChunks; x++) {
			for (int z = 0; z < numberOfChunks; z++) {
				GenerateChunkAt(XMFLOAT3(x, 0, z));
			}
		}
	}
	else {
		GenerateChunkIfWeNeedTo();
	}
	return true;
}

void SceneNodeTerrain::Render()
{

	//W
	if (GetAsyncKeyState(0x57) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetForwardBack(1);
		if (generateDynamically)
			GenerateChunkIfWeNeedTo();
	} // S
	else if (GetAsyncKeyState(0x53) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetForwardBack(-1);
		if (generateDynamically)
			GenerateChunkIfWeNeedTo();
	}

	//A 
	if (GetAsyncKeyState(0x41) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetLeftRight(-1);
		if (generateDynamically)
			GenerateChunkIfWeNeedTo();
	} // D
	else if (GetAsyncKeyState(0x44) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetLeftRight(1);
		if (generateDynamically)
			GenerateChunkIfWeNeedTo();
	}
	if (GetAsyncKeyState(VK_UP) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetPitch(1);
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetPitch(-1);
	}

	if (GetAsyncKeyState(VK_LEFT) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetYaw(-1);
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetYaw(1);
	}

	//useFreecam = GetAsyncKeyState(VK_LSHIFT) < 0;

	if(!useFreecam)
		GetChunkPlayerIsIn().data->LevelCamera();
}

void SceneNodeTerrain::Shutdown()
{
	/*
	for (auto c : chunks) {
		delete c.data;
	}
	*/
}

void SceneNodeTerrain::SetSceneGraph(SceneGraph* ptr)
{
	this->sceneGraph = ptr;
}

void SceneNodeTerrain::GenerateChunkAt(XMFLOAT3 position)
{
	SceneNodeChunk* chunk = new SceneNodeChunk(L"Chunk");
	chunk->GenerateTerrain(position, sceneGraph);
	chunk->SetWorldTransform(XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(position.x * chunkSize, position.y, position.z * chunkSize));
	string id = "chunk_" + std::to_string(position.x) + "_" + std::to_string(position.z);
	chunks.push_back({ id, position, chunk });
}

void SceneNodeTerrain::GenerateChunkIfWeNeedTo()
{
	float camX = ChunkX();
	float camZ = ChunkZ();

	for (float x = camX - viewSize; x <= camX + viewSize; x++) {
		for (float z = camZ - viewSize; z <= camZ + viewSize; z++) {
			if (!ChunkExistsAt(XMFLOAT3(x, 0, z))) {
				GenerateChunkAt(XMFLOAT3(x, 0, z));
			}
		}
	}
}

// This is the issue
bool SceneNodeTerrain::ChunkExistsAt(XMFLOAT3 position)
{
	string id = "chunk_" + std::to_string(position.x) + "_" + std::to_string(position.z);
	for (Chunk chunk : chunks) {
		if (chunk.id.compare(id) == 0) {
			return true;
		}
	}
	return false;
}

float SceneNodeTerrain::ChunkX(void)
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	// Equation = round ( cam x / (tile scale * chunk size)
	return roundf(position.x / (2 * chunkSize));
}

float SceneNodeTerrain::ChunkZ(void)
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	// Equation = round ( cam z / (tile scale * chunk size)
	return roundf(position.z / (2 * chunkSize));
}

SceneNodeTerrain::Chunk SceneNodeTerrain::GetChunkPlayerIsIn()
{
	float camX = ChunkX();
	float camZ = ChunkZ();
	if (ChunkExistsAt(XMFLOAT3(camX, 0, camZ))) {
		string id = "chunk_" + std::to_string(camX) + "_" + std::to_string(camZ);
		for (Chunk chunk : chunks) {
			if (chunk.id.compare(id) == 0) {
				return chunk;
			}
		}
	}
	//Default to first chunk
	return chunks[0];
}
