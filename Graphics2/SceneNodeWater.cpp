/*
	Author: Luke Rapkin
*/
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
	// Create wave effects
	// Should really be doing in a shader...
	/* Does C++/DirectX not let you modify the vertices once the geometry is built? - Apparently not
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

	//Blending
	SetupBlendState(new float[4]{ 1.0f, 1.0f, 1.0f, 1.0f }, 0xffffffff);
	BuildBlendState();

	// Blending works but theres a tiny glitch at chunk seams - The terrain IS there but for some reason the blending hides it
	useBlending = true;

	texture = L"textures/water.png";
}
