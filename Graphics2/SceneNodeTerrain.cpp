#include "SceneNodeTerrain.h"
#include "SceneNodeWater.h"
#include "SceneNodeChunk.h"

bool SceneNodeTerrain::Initialise()
{
	//For demoing
	if (!generateDynamically) {
		/*
		for (int x = 0; x < 1; x++) {
			for (int z = 0; z < 1; z++) {
				GenerateChunkAt(XMFLOAT3(x, 0, z));
			}
		}
		*/
		GenerateChunkAt(XMFLOAT3(0, 0, 0));
	}
	else {
		std::thread generator([&] {
			while (true) {
				bool condition = (GetAsyncKeyState(VK_UP) < 0) || (GetAsyncKeyState(VK_DOWN) < 0) || (GetAsyncKeyState(VK_LEFT) < 0) || (GetAsyncKeyState(VK_RIGHT) < 0);
				if(condition)
					GenerateChunkIfWeNeedTo();
			}
			});

		generator.detach();
	}
	return true;
}

void SceneNodeTerrain::Render()
{

	if (GetAsyncKeyState(VK_UP) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetForwardBack(1);
		//GenerateChunkIfWeNeedTo();
	}
	else if (GetAsyncKeyState(VK_DOWN) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetForwardBack(-1);
		//GenerateChunkIfWeNeedTo();
	}

	if (GetAsyncKeyState(VK_LEFT) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetLeftRight(-1);
		//GenerateChunkIfWeNeedTo();
	}
	else if (GetAsyncKeyState(VK_RIGHT) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetLeftRight(1);
		//GenerateChunkIfWeNeedTo();
	}

	if (GetAsyncKeyState(VK_NUMPAD8) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetPitch(1);
	}
	else if (GetAsyncKeyState(VK_NUMPAD2) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetPitch(-1);
	}

	if (GetAsyncKeyState(VK_NUMPAD4) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetYaw(-1);
	}
	else if (GetAsyncKeyState(VK_NUMPAD6) < 0) {
		DirectXFramework::GetDXFramework()->GetCamera()->SetYaw(1);
	}
}

void SceneNodeTerrain::Shutdown()
{
}

void SceneNodeTerrain::SetSceneGraph(SceneGraph* ptr)
{
	this->sceneGraph = ptr;
}

void SceneNodeTerrain::GenerateChunkAt(XMFLOAT3 position)
{
	SceneNodeChunk* chunk = new SceneNodeChunk(L"Chunk");
	chunk->GenerateTerrain(position, sceneGraph);
	sceneGraph->Add(chunk);
	chunk->SetWorldTransform(XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(position.x * chunkSize, position.y, position.z * chunkSize));
	string id = "chunk_" + std::to_string(position.x) + "_" + std::to_string(position.z);
	chunks.push_back({ id, position });
}

void SceneNodeTerrain::GenerateChunkIfWeNeedTo()
{
	if (!generateDynamically) return;

	float camX = ChunkX();
	float camZ = ChunkZ();

	for (float x = camX - viewSize; x <= camX + viewSize; x++) {
		for (float z = camZ - viewSize; z <= camZ + viewSize; z++) {
			if (!ChunkExistsAt(XMFLOAT3(x, 0, z))) {
				/*
				std::thread generator([&] {
					GenerateChunkAt(XMFLOAT3(x, 0, z));
					});

				generator.detach();
				*/
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
	return roundf(position.x / (1 * chunkSize));
}

float SceneNodeTerrain::ChunkZ(void)
{
	XMVECTOR cameraPosition = DirectXFramework::GetDXFramework()->GetCamera()->GetCameraPosition();
	XMFLOAT4 position;
	XMStoreFloat4(&position, cameraPosition);
	return roundf(position.z / (1 * chunkSize));
}
