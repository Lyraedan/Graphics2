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

	/*
	//Blending
	SetupBlendState(new float[4]{ 0.0f, 0.0f, 0.0f, 0.0f }, 0xffffffff);
	D3D11_RENDER_TARGET_BLEND_DESC desc;
	desc.BlendEnable = TRUE;
	desc.SrcBlend = D3D11_BLEND_ONE;
	desc.DestBlend = D3D11_BLEND_ONE;
	desc.BlendOp = D3D11_BLEND_OP_ADD;
	desc.SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BuildBlendState(desc);
	useBlending = true;
	*/

	texture = L"water.bmp";
}
