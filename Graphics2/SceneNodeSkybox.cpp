#include "SceneNodeSkybox.h"

bool SceneNodeSkybox::Initialise()
{
	_device = DirectXFramework::GetDXFramework()->GetDevice();
	_deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	if (_device == nullptr || _deviceContext == nullptr) {
		return false;
	}
	// Load our plane model into memory
	objLoader.LoadModel("plane/skybox.obj");

	BuildGeometry();
	BuildShaders(); // causes crash without hlsl shader
	BuildVertexLayout(); // causes crash fixed after adding hlsl shader
	BuildConstantBuffer();
	BuildRendererState(D3D11_CULL_NONE);
	BuildTexture();
	return true;
}


float skyY = 0;
void SceneNodeSkybox::Tick()
{
	skyY++;
	float skyScl = 150;
	SetWorldTransform(XMMatrixScaling(skyScl, skyScl, skyScl) * XMMatrixTranslation(0, 0, 0) * XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), skyY * 0.5f * XM_PI / 180.0f));

}


void SceneNodeSkybox::Render()
{
	XMMATRIX view = DirectXFramework::GetDXFramework()->GetViewTransformation();
	XMMATRIX proj = DirectXFramework::GetDXFramework()->GetProjectionTransformation();
	XMMATRIX comp = XMLoadFloat4x4(&_combinedWorldTransformation) * view * proj;

	Tick();

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
	_deviceContext->DrawIndexed(vertices.size() * 2, 0, 0); // This line is causing a crash before implementing shaders, vertex layout, constant buffer
	
	_deviceContext->RSSetState(_defaultRasteriserState.Get());
}

void SceneNodeSkybox::BuildGeometry() {
	for(ObjLoader::Face face : objLoader.faces) {
		for (XMFLOAT3 v : face.faceData) {
			XMFLOAT3 verts = XMFLOAT3(objLoader.verts.at((int) v.x - 1));
			XMFLOAT2 uv = XMFLOAT2(objLoader.uvs.at((int) v.y - 1));
			XMFLOAT3 normals = XMFLOAT3(objLoader.normals.at((int) v.z - 1));
			AddVertex(verts, normals, uv);
		}

		for (XMFLOAT3 tris : face.tris) {
			AddIndice(tris.x, tris.y, tris.z);
		}
	}

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

void SceneNodeSkybox::BuildConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CBUFFER);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(_device->CreateBuffer(&desc, NULL, constantBuffer.GetAddressOf()));
}

void SceneNodeSkybox::BuildVertexLayout()
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

void SceneNodeSkybox::BuildShaders() {
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

void SceneNodeSkybox::BuildTexture()
{
	ThrowIfFailed(CreateWICTextureFromFile(_device.Get(),
		_deviceContext.Get(),
		L"plane/skybox_temp.png",
		nullptr,
		_texture.GetAddressOf()
	));

}

void SceneNodeSkybox::AddVertex(XMFLOAT3 position, XMFLOAT3 normals, XMFLOAT2 uv)
{
	vertices.push_back({ position,  normals,  uv });
}

void SceneNodeSkybox::AddIndice(UINT p1, UINT p2, UINT p3)
{
	indices.push_back({ p1, p2, p3 });
}

void SceneNodeSkybox::BuildRendererState(D3D11_CULL_MODE mode)
{
	// Set default and no cull rasteriser states
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

void SceneNodeSkybox::Shutdown()
{

}