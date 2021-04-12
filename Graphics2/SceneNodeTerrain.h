#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "PerlinNoise.h"
#include <math.h>

class SceneNodeTerrain : public SceneNode
{
	struct Chunk {
		XMFLOAT3 position;
	};

public:
	SceneNodeTerrain(wstring name) : SceneNode(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void GenerateTerrain(XMFLOAT3 terrainOffset);
	void SetSceneGraph(SceneGraphPointer ptr);

	void GenerateChunkIfWeNeedTo();

	bool ChunkExistsAt(XMFLOAT3 position);
	float ChunkX(void);
	float ChunkZ(void);

	int chunkSize = 50; //50
	float terrain[50][50];
	float viewSize = 1;

private:
	// making this & breaks the constructor
	SceneGraphPointer sceneGraph;
	std::vector<Chunk> chunks;
	void UpdateHeight(float xOffset, float zOffset);

};
