/*
	Author: Luke Rapkin
*/
#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "SceneNodeWater.h"
#include "SceneNodeChunk.h"
#include <math.h>
#include <thread>

class SceneNodeTerrain : public SceneNode
{
public:
	struct Chunk {
		string id;
		XMFLOAT3 position;
		SceneNodeChunk* data;
	};

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
	Chunk GetChunkPlayerIsIn();

	int chunkSize = 16; //50
	float viewSize = 3;
	// Enable or disable infinite terrain
	bool generateDynamically = true;
	bool useFreecam = true;

private:
	// making this & breaks the constructor
	SceneGraph* sceneGraph;
	std::vector<Chunk> chunks;
	std::vector<SceneNodeChunk*> chunkPtrs;
	std::thread chunkThread;
};
