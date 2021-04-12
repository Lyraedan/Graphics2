#include "SceneNodeMesh.h"

bool SceneNodeMesh::Initialise()
{
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	if (_device == nullptr || _deviceContext == nullptr) {
		return false;
	}
	SetupMesh();
	BuildGeometry();
	BuildShaders();
	BuildVertexLayout();
	BuildConstantBuffer();
	BuildTexture();
	return true;
}


void SceneNodeMesh::Tick(XMMATRIX& completeTransform) { }


void SceneNodeMesh::Render()
{
	XMMATRIX view = DirectXFramework::GetDXFramework()->GetCamera()->GetViewMatrix();
	XMMATRIX proj = DirectXFramework::GetDXFramework()->GetProjectionTransformation();
	XMMATRIX comp = XMLoadFloat4x4(&_combinedWorldTransformation) * view * proj;

	Tick(comp);

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
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->DrawIndexed(vertices.size() * 2, 0, 0);

	_deviceContext->RSSetState(_defaultRasteriserState.Get());
}

void SceneNodeMesh::SetupMesh() { }

void SceneNodeMesh::BuildGeometry() {
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices.data(); //objLoader.verts.data();

	ThrowIfFailed(_device->CreateBuffer(&vbd, &vinitData, &vertexBuffer));

	// Index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (indices.size() * 3);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices.data();

	ThrowIfFailed(_device->CreateBuffer(&ibd, &iinitData, &indexBuffer));
}

void SceneNodeMesh::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CBUFFER);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&desc, NULL, constantBuffer.GetAddressOf()));
}

void SceneNodeMesh::BuildVertexLayout()
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

void SceneNodeMesh::BuildShaders() {
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

void SceneNodeMesh::BuildTexture()
{
	ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
		_deviceContext.Get(),
		texture,
		nullptr,
		_texture.GetAddressOf()
	));

}

void SceneNodeMesh::AddVertex(XMFLOAT3 position, XMFLOAT3 normals, XMFLOAT2 uv)
{
	vertices.push_back({ position,  normals,  uv });
}

void SceneNodeMesh::AddIndice(UINT p1, UINT p2, UINT p3)
{
	indices.push_back({ p1, p2, p3 });
}

void SceneNodeMesh::BuildRendererState(D3D11_CULL_MODE mode)
{
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	rasteriserDesc.CullMode = mode;
	rasteriserDesc.FrontCounterClockwise = true;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _defaultRasteriserState.GetAddressOf()));
	rasteriserDesc.CullMode = D3D11_CULL_NONE;
	ThrowIfFailed(_device->CreateRasterizerState(&rasteriserDesc, _noCullRasteriserState.GetAddressOf()));
}

void SceneNodeMesh::Shutdown()
{

}