#pragma once

#include "SceneNode.h"
#include "Graphics2.h"
#include "WICTextureLoader.h"
#include <list>

class SceneNodeMesh : public SceneNode
{
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

	struct Indice
	{
		UINT p1;
		UINT p2;
		UINT p3;
	};

public:
	SceneNodeMesh(wstring name) : SceneNode(name) {};

	bool Initialise() override;
	virtual void Render() override;
	virtual void Shutdown() override;
	virtual void Tick(XMMATRIX& completeTransform);
	virtual void SetupMesh();

	void AddVertex(XMFLOAT3 position, XMFLOAT3 normals, XMFLOAT2 uvs);
	void AddIndice(UINT  p1, UINT  p2, UINT p3);

	const wchar_t* texture = L"";
private:
	void BuildGeometry();
	void BuildConstantBuffer();
	void BuildVertexLayout();
	void BuildShaders();
	void BuildTexture();

	XMMATRIX worldTransformation;
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<Indice> indices = std::vector<Indice>();
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
