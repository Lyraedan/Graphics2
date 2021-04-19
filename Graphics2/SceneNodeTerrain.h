#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "PerlinNoise.h"
#include <math.h>
#include <thread>

class SceneNodeTerrain : public SceneNode
{
	struct Chunk {
		string id;
		XMFLOAT3 position;
	};

public:
	SceneNodeTerrain(wstring name) : SceneNode(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void SetSceneGraph(SceneGraph* ptr);

	void GenerateChunkAt(XMFLOAT3 position);
	void GenerateChunkIfWeNeedTo();

	bool ChunkExistsAt(XMFLOAT3 position);
	float ChunkX(void);
	float ChunkZ(void);

	int chunkSize = 16; //50
	float viewSize = 1;
	bool generateDynamically = false;

private:
	// making this & breaks the constructor
	SceneGraph* sceneGraph;
	std::vector<Chunk> chunks;
	std::thread chunkThread;
};
