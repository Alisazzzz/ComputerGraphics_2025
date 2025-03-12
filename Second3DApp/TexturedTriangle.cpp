#include "TexturedTriangle.h"
#include "Game.h"

void TexturedTriangle::Initialize(LPCWSTR shaderSource, 
	std::vector<Vertex> pointsInput, std::vector<int> indecesInput, 
	std::vector<UINT> stridesInput, std::vector<UINT> offsetsInput, bool is2DInput)
{
	points = pointsInput;
	indeces = indecesInput;

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

	D3D11_INPUT_ELEMENT_DESC InputElements[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
	D3D11_INPUT_ELEMENT_DESC {
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0}
	};

	game->device->CreateInputLayout(
		InputElements,
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
	vertexBufDesc.ByteWidth = sizeof(Vertex) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&indexBufDesc, &indexData, &ib);

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

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; 
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerState = nullptr;
	game->device->CreateSamplerState(&samplerDesc, &samplerState);
}

void TexturedTriangle::Draw()
{
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);

	game->context->PSSetShaderResources(0, 1, &textureView);
	game->context->PSSetSamplers(0, 1, &samplerState);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());
	game->context->VSSetShader(vertexShader, nullptr, 0);
	game->context->PSSetShader(pixelShader, nullptr, 0);

	game->context->DrawIndexed(indeces.size(), 0, 0);
}

void TexturedTriangle::Update()
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

void TexturedTriangle::DestroyResources()
{
	layout->Release();

	textureView->Release();
	samplerState->Release();

	vertexShader->Release();
	vertexByteCode->Release();
	vb->Release();

	pixelShader->Release();
	pixelByteCode->Release();

	ib->Release();

	rastState->Release();
}
