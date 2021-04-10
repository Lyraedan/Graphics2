#include "SceneNodeTerrain.h"
#include "PerlinNoise.h"

bool SceneNodeTerrain::Initialise()
{
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	if (_device == nullptr || _deviceContext == nullptr) {
		return false;
	}
	BuildGeometry();
	BuildShaders(); // causes crash without hlsl shader
	BuildVertexLayout(); // causes crash fixed after adding hlsl shader
	BuildConstantBuffer();
	BuildTexture();
    return true;
}

void SceneNodeTerrain::Render()
{
	XMMATRIX view = DirectXFramework::GetDXFramework()->GetViewTransformation();
	XMMATRIX proj = DirectXFramework::GetDXFramework()->GetProjectionTransformation();
	XMMATRIX comp = XMLoadFloat4x4(&_combinedWorldTransformation) * view * proj;

	CBUFFER cBuffer;
	cBuffer.CompleteTransformation = comp;
	cBuffer.WorldTransformation = XMLoadFloat4x4(&_worldTransformation);
	cBuffer.AmbientColour = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	cBuffer.LightVector = XMVector4Normalize(XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f));
	cBuffer.LightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	_deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cBuffer, 0, 0);

	// Set the texture to be used by the pixel shader
	_deviceContext->PSSetShaderResources(0, 1, _texture.GetAddressOf());


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // draw triangles
	_deviceContext->DrawIndexed(36, 0, 0); // This line is causing a crash before implementing shaders, vertex layout, constant buffer
}

// What im trying to achieve: world * view * projection
//										= view * projection

void SceneNodeTerrain::BuildGeometry() {

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices.data();

	ThrowIfFailed(_device->CreateBuffer(&vbd, &vinitData, &vertexBuffer));

	// Create the index buffer
	UINT indices[] = {
						0, 1, 2,
						2, 1, 3
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	ThrowIfFailed(_device->CreateBuffer(&ibd, &iinitData, &indexBuffer));
}

void SceneNodeTerrain::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CBUFFER);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&desc, NULL, constantBuffer.GetAddressOf()));
}

void SceneNodeTerrain::BuildVertexLayout()
{
	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), _layout.GetAddressOf()));
	_deviceContext->IASetInputLayout(_layout.Get());
}

void SceneNodeTerrain::BuildShaders() {
	DWORD shaderCompileFlags = 0;
#if defined( _DEBUG )
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> compilationMessages = nullptr;

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile(L"shader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderCompileFlags, 0,
		_vertexShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreateVertexShader(_vertexShaderByteCode->GetBufferPointer(), _vertexShaderByteCode->GetBufferSize(), NULL, _vertexShader.GetAddressOf()));
	_deviceContext->VSSetShader(_vertexShader.Get(), 0, 0);

	// Compile pixel shader
	hr = D3DCompileFromFile(L"shader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderCompileFlags, 0,
		_pixelShaderByteCode.GetAddressOf(),
		compilationMessages.GetAddressOf());

	if (compilationMessages.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)compilationMessages->GetBufferPointer(), 0, 0);
	}
	ThrowIfFailed(hr);
	ThrowIfFailed(_device->CreatePixelShader(_pixelShaderByteCode->GetBufferPointer(), _pixelShaderByteCode->GetBufferSize(), NULL, _pixelShader.GetAddressOf()));
	_deviceContext->PSSetShader(_pixelShader.Get(), 0, 0);
}

void SceneNodeTerrain::BuildTexture()
{
	ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
		_deviceContext.Get(),
		L"grass.bmp",
		nullptr,
		_texture.GetAddressOf()
	));

}

void SceneNodeTerrain::AddVertex(XMFLOAT3 position, XMFLOAT3 normals, XMFLOAT2 uv)
{
	vertices.push_back({ position,  normals,  uv });
}

void SceneNodeTerrain::Shutdown()
{
}