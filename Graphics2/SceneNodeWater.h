#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "WICTextureLoader.h"
#include <list>

class SceneNodeWater : public SceneNode
{
	//Gerstner wave

	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 TextureCoordinate;
	};

	struct CBUFFER
	{
		XMMATRIX    CompleteTransformation;
		XMMATRIX	WorldTransformation;
		XMVECTOR    LightVector;
		XMFLOAT4    LightColour;
		XMFLOAT4    AmbientColour;
	};

public:
	SceneNodeWater(wstring name) : SceneNode(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;

	void BuildGeometry();
	void BuildConstantBuffer();
	void BuildVertexLayout();
	void BuildShaders();
	void BuildTexture();

	void AddVertex(XMFLOAT3 position, XMFLOAT3 normals, XMFLOAT2 uvs);

private:
	XMMATRIX worldTransformation;
	std::vector<Vertex> vertices = std::vector<Vertex>();
	ComPtr<ID3D11Buffer>			vertexBuffer;
	ComPtr<ID3D11Buffer>			indexBuffer;
	ComPtr<ID3D11Buffer>			constantBuffer;
	ComPtr<ID3D11InputLayout>		_layout;
	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;

	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

