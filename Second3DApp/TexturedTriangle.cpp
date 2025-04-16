#include "TexturedTriangle.h"
#include "Game.h"
#include "MeshGenerator.h"

void TexturedTriangle::CreateShadowShaders()
{
	ID3DBlob* errorVertexCode = nullptr;
	HRESULT res = D3DCompileFromFile(L"./Shaders/ShadowMapShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexByteCode_shadows,
		&errorVertexCode);
	game->device->CreateVertexShader(
		vertexByteCode_shadows->GetBufferPointer(),
		vertexByteCode_shadows->GetBufferSize(),
		nullptr, &vertexShader_shadows);


	ID3DBlob* errorPixelCode = nullptr;
	res = D3DCompileFromFile(L"./Shaders/ShadowMapShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelByteCode_shadows,
		&errorPixelCode);
	game->device->CreatePixelShader(
		pixelByteCode_shadows->GetBufferPointer(),
		pixelByteCode_shadows->GetBufferSize(),
		nullptr, &pixelShader_shadows);

	D3D11_SAMPLER_DESC shadowSamplerDesc;
	ZeroMemory(&shadowSamplerDesc, sizeof(shadowSamplerDesc));

	shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.BorderColor[0] = 0;
	shadowSamplerDesc.BorderColor[1] = 0;
	shadowSamplerDesc.BorderColor[2] = 0;
	shadowSamplerDesc.BorderColor[3] = 0;
	shadowSamplerDesc.MinLOD = 0;
	shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSamplerDesc.MaxAnisotropy = 1;
	shadowSamplerDesc.MipLODBias = 0.0f;

	game->device->CreateSamplerState(&shadowSamplerDesc, &shadowSampler);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode =  D3D11_FILL_SOLID /*D3D11_FILL_WIREFRAME*/;
	rastDesc.DepthBias = 20000;
	rastDesc.SlopeScaledDepthBias = 1.0f;
	rastDesc.DepthBiasClamp = 0.0f;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState_shadows);
}

void TexturedTriangle::CreateShadowVolumesShaders()
{
	ID3DBlob* errorVertexCode = nullptr;
	HRESULT res = D3DCompileFromFile(L"./Shaders/ShadowVolumesShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexByteCode_shadowVolumes,
		&errorVertexCode);
	game->device->CreateVertexShader(
		vertexByteCode_shadowVolumes->GetBufferPointer(),
		vertexByteCode_shadowVolumes->GetBufferSize(),
		nullptr, &vertexShader_shadowVolumes);


	ID3DBlob* errorGeometryCode = nullptr;
	res = D3DCompileFromFile(L"./Shaders/ShadowVolumesShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"GSMain",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&geometryByteCode_shadowVolumes,
		&errorGeometryCode);
	game->device->CreateGeometryShader(
		geometryByteCode_shadowVolumes->GetBufferPointer(),
		geometryByteCode_shadowVolumes->GetBufferSize(),
		nullptr, &geometryShader_shadowVolumes);

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.ByteWidth = sizeof(UINT) * indeces_with_adjastency.size();
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &indeces_with_adjastency[0];

	game->device->CreateBuffer(&ibDesc, &initData, &pAdjacencyIB);
}

void TexturedTriangle::Initialize(LPCWSTR shaderSource,
	std::vector<Vertex> pointsInput, std::vector<int> indecesInput, 
	bool is2DInput, std::wstring texturePath, Material* materialInput)
{
	points = pointsInput;
	indeces = indecesInput;

	strides = { 36 };
	offsets = { 0 };

	is2D = is2DInput;
	material = materialInput;

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

	ID3DBlob* errorPixelCode = nullptr;

	res = D3DCompileFromFile(shaderSource,
		nullptr /*macros*/,
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
		D3D11_INPUT_ELEMENT_DESC { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	game->device->CreateInputLayout(
		InputElements,
		3,
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
		rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID /*D3D11_FILL_WIREFRAME*/;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState);

	//Constant buffers: transforms
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

	//Constant buffers: light
	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	lightBufferDesc.ByteWidth = sizeof(LightData);

	game->device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);

	lightData = {};

	//Texture
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

	DirectX::ScratchImage image;
	DirectX::TexMetadata metadata;

	DirectX::LoadFromWICFile(texturePath.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, image);
	metadata = image.GetMetadata();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = static_cast<UINT>(metadata.width);
	textureDesc.Height = static_cast<UINT>(metadata.height);
	textureDesc.MipLevels = static_cast<UINT>(metadata.mipLevels);
	textureDesc.ArraySize = 1;
	textureDesc.Format = metadata.format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* texture2D = nullptr;
	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = image.GetPixels();
	textureData.SysMemPitch = static_cast<UINT>(image.GetImages()->rowPitch);

	res = game->device->CreateTexture2D(&textureDesc, &textureData, &texture2D);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	res = game->device->CreateShaderResourceView(texture2D, &srvDesc, &textureView);
	texture2D->Release();	

	//FOR SHADOW VOLUMES
	indeces_with_adjastency = MeshGenerator::getInstance()->GenerateAdjastencyIndices(points, indeces);
	res = D3DCompileFromFile(L"./Shaders/MySecondShader_withoutLights.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelByteCode_withoutLights,
		&errorPixelCode);

	game->device->CreatePixelShader(
		pixelByteCode_withoutLights->GetBufferPointer(),
		pixelByteCode_withoutLights->GetBufferSize(),
		nullptr, &pixelShader_withoutLights);
}

void TexturedTriangle::Draw()
{
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);
	game->context->PSSetConstantBuffers(1, 1, &lightBuffer);

	game->context->PSSetShaderResources(0, 1, &textureView);
	game->context->PSSetSamplers(0, 1, &samplerState);	
	
	//shadowsResource = game->dirLightShadows->GetShadowMapDSV();
	//game->context->PSSetShaderResources(1, 1, &shadowsResource);
	//game->context->PSSetSamplers(1, 1, &shadowSampler);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());

	game->context->VSSetShader(vertexShader, nullptr, 0);
	game->context->GSSetShader(nullptr, nullptr, 0);
	game->context->PSSetShader(pixelShader, nullptr, 0);

	game->context->DrawIndexed(indeces.size(), 0, 0);
}

void TexturedTriangle::LightRender()
{
	game->context->RSSetState(rastState_shadows);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());
	game->context->VSSetShader(vertexShader_shadows, nullptr, 0);
	game->context->GSSetShader(nullptr, nullptr, 0);
	game->context->PSSetShader(pixelShader_shadows, nullptr, 0);

	game->context->DrawIndexed(indeces.size(), 0, 0);
}

//FOR SHADOW VOLUMES
void TexturedTriangle::RenderWithoutLight()
{
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);
	game->context->PSSetConstantBuffers(1, 1, &lightBuffer);

	game->context->PSSetShaderResources(0, 1, &textureView);
	game->context->PSSetSamplers(0, 1, &samplerState);

	game->context->PSSetShaderResources(1, 1, &shadowsResource);
	game->context->PSSetSamplers(1, 1, &shadowSampler);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());
	game->context->VSSetShader(vertexShader, nullptr, 0);
	game->context->GSSetShader(nullptr, nullptr, 0);
	game->context->PSSetShader(pixelShader_withoutLights, nullptr, 0);

	game->context->DrawIndexed(indeces.size(), 0, 0);
}

void TexturedTriangle::CreateShadowVolumes()
{
	game->context->RSSetState(rastState);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
	game->context->IASetIndexBuffer(pAdjacencyIB, DXGI_FORMAT_R32_UINT, 0);

	game->context->VSSetConstantBuffers(0, 1, &constBuffer);
	game->context->GSSetConstantBuffers(0, 1, &constBuffer);
	game->context->GSSetConstantBuffers(1, 1, &lightBuffer);

	game->context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());
	game->context->VSSetShader(vertexShader_shadowVolumes, nullptr, 0);
	game->context->GSSetShader(geometryShader_shadowVolumes, nullptr, 0);
	game->context->PSSetShader(nullptr, nullptr, 0);

	game->context->DrawIndexed(indeces_with_adjastency.size(), 0, 0);
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

	if (game->pntLights.size() != 0) {
		for (int i = 0; i < game->pntLights.size(); i++) {
			lightData.points[i] = *(game->pntLights[i]);
		};
	};

	if (game->dirLight != nullptr)
		lightData.directional = *(game->dirLight);

	lightData.material = *material;
	lightData.spectator = Vector4(game->activeCamera->lookPoint.x, game->activeCamera->lookPoint.y, game->activeCamera->lookPoint.z, 1.0f);

	lightData.lightSpace = game->lightView * game->lightProjection;
	lightData.lightSpace = lightData.lightSpace.Transpose();

	D3D11_MAPPED_SUBRESOURCE resLight = {};
	game->context->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resLight);
	memcpy(resLight.pData, &lightData, sizeof(LightData));
	game->context->Unmap(lightBuffer, 0);
}

void TexturedTriangle::LightUpdate()
{
	constData.transformations = transforms.scale * transforms.rotate * transforms.move;
	constData.transformations = constData.transformations.Transpose();

	constData.view = game->lightView;
	constData.view = constData.view.Transpose();

	constData.projection = game->lightProjection;
	constData.projection = constData.projection.Transpose();

	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, &constData, sizeof(ConstData));
	game->context->Unmap(constBuffer, 0);
}

void TexturedTriangle::DestroyResources()
{
	layout->Release();
	
	if (textureView != nullptr)
		textureView->Release();
	samplerState->Release();

	vertexShader->Release();
	vertexByteCode->Release();
	vb->Release();

	pixelShader->Release();
	pixelByteCode->Release();

	ib->Release();

	rastState->Release();

	if (shadowSampler != nullptr) {
		shadowSampler->Release();

		rastState_shadows->Release();

		vertexShader_shadows->Release();
		vertexByteCode_shadows->Release();

		pixelShader_shadows->Release();
		pixelByteCode_shadows->Release();
	};

	if (material != nullptr)
		delete material;
}
