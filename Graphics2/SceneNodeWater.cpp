#include "SceneNodeWater.h"

bool SceneNodeWater::Initialise()
{
	return SceneNodeMesh::Initialise();
}

void SceneNodeWater::Shutdown()
{
}

void SceneNodeWater::Tick(XMMATRIX& completeTransform)
{
	// Create wave affects
	// Should really be doing in a shader...
	/* Does C++/DirectX not let you modify the vertices once the geometry is built?
	for (int i = 0; i < GetVertices(); i++) {
		SceneNodeMesh::Vertex vert = GetVertex(i);
		float wave = sin(vert.Position.x + waveY) * waveAmplitude;
		vert.Position.y = wave;
	}
	waveY += waveSpeed;
	*/
}

void SceneNodeWater::SetupMesh()
{
	BuildRendererState(D3D11_CULL_FRONT);
	BuildBlendState();
	SetupBlendState(new float[4] {0.5f, 0.5f, 0.5f, 0.5f}, 0xffffffff);
	texture = L"water.bmp";
}
