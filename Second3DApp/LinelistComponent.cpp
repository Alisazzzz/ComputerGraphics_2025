#include "LinelistComponent.h"

#include "Game.h"

void LinelistComponent::Initialize(LPCWSTR shaderSource,
	std::vector<DirectX::XMFLOAT4> pointsInput,
	std::vector<UINT> stridesInput, std::vector<UINT> offsetsInput, bool is2DInput)
{
	points = pointsInput;

	strides = stridesInput;
	offsets = offsetsInput;

	is2D = is2DInput;

	ID3DBlob* errorVertexCode = nullptr;
	HRESULT res = D3DCompileFromFile(shaderSource,
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexByteCode,
		&errorVertexCode);

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
	ID3DBlob* errorPixelCode = nullptr;

	res = D3DCompileFromFile(shaderSource,
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelByteCode,
		&errorPixelCode);

	game->device->CreateVertexShader(
		vertexByteCode->GetBufferPointer(),
		vertexByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->device->CreatePixelShader(
		pixelByteCode->GetBufferPointer(),
		pixelByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
	D3D11_INPUT_ELEMENT_DESC {
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0}
	};

	game->device->CreateInputLayout(
		inputElements,
		2,
		vertexByteCode->GetBufferPointer(),
		vertexByteCode->GetBufferSize(),
		&layout);

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	if (is2D)
		rastDesc.CullMode = D3D11_CULL_NONE;
	else
		rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID /*D3D11_FILL_WIREFRAME*/;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState);

	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;
	constBufferDesc.ByteWidth = sizeof(ConstData);

	game->device->CreateBuffer(&constBufferDesc, nullptr, &constBuffer);

	transforms = {};
	transforms.move = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	transforms.rotate = Matrix::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f);
	transforms.scale = Matrix::CreateScale(1.0f, 1.0f, 1.0f);

	constData = {};
	constData.transformations = transforms.scale * transforms.rotate * transforms.move;
	constData.color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void LinelistComponent::Draw()
{
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());
	game->context->VSSetShader(vertexShader, nullptr, 0);
	game->context->PSSetShader(pixelShader, nullptr, 0);

	game->context->Draw(points.size() / 4.0f, 0);
}

void LinelistComponent::Update()
{
	constData.transformations = transforms.scale * transforms.rotate * transforms.move;
	constData.transformations = constData.transformations.Transpose();

	constData.view = game->activeCamera->cameraInfo.view;
	constData.view = constData.view.Transpose();

	constData.projection = game->activeCamera->cameraInfo.projection;
	constData.projection = constData.projection.Transpose();

	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &constData, sizeof(ConstData));
	game->context->Unmap(constBuffer, 0);
}

void LinelistComponent::DestroyResources()
{
	layout->Release();

	vertexShader->Release();
	vertexByteCode->Release();
	vb->Release();

	pixelShader->Release();
	pixelByteCode->Release();

	rastState->Release();
}
