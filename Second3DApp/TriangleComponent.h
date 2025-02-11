#pragma once

#include <wrl.h>
#include <iostream>
#include <algorithm>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class TriangleComponent
{
private:
	int CompileShader(LPCWSTR shaderFile,
		const D3D_SHADER_MACRO* shaderMacros,
		ID3DInclude* pInclude,
		LPCSTR entryPoint,
		LPCSTR shaderType,
		UINT flags1,
		UINT flags2,
		ID3DBlob* code,
		ID3DBlob* errorMsg,
		HWND hWnd);

public:
	ID3D11InputLayout* layout;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexByteCode;
	ID3D11Buffer* vb;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelByteCode;

	ID3D11Buffer* ib;
	ID3D11RasterizerState* rastState;

	TriangleComponent(Microsoft::WRL::ComPtr<ID3D11Device> device,
		LPCWSTR shaderFile,
		const D3D11_INPUT_ELEMENT_DESC* inputElements,
		HWND hWnd
		//DirectX::XMFLOAT4 pointsInput[], int pointsInputSize,
		//int indecesInput[], int indecesInputSize
		)
	{
		ID3DBlob* vertexByteCode = nullptr;
		ID3DBlob* errorVertexCode = nullptr;
		HRESULT res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
			nullptr /*macros*/,
			nullptr /*include*/,
			"VSMain",
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&vertexByteCode,
			&errorVertexCode);


		D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
		pixelByteCode = nullptr;
		ID3DBlob* errorPixelCode = nullptr;

		res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", 
			Shader_Macros /*macros*/,
			nullptr /*include*/, 
			"PSMain", 
			"ps_5_0", 
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
			0, 
			&pixelByteCode, 
			&errorPixelCode);

		device->CreateVertexShader(
			vertexByteCode->GetBufferPointer(),
			vertexByteCode->GetBufferSize(),
			nullptr, &vertexShader);

		device->CreatePixelShader(
			pixelByteCode->GetBufferPointer(),
			pixelByteCode->GetBufferSize(),
			nullptr, &pixelShader);

		device->CreateInputLayout(
			inputElements,
			2,
			vertexByteCode->GetBufferPointer(),
			vertexByteCode->GetBufferSize(),
			&layout);

		DirectX::XMFLOAT4 points[8] = {
			DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		};

		D3D11_BUFFER_DESC vertexBufDesc = {};
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufDesc.CPUAccessFlags = 0;
		vertexBufDesc.MiscFlags = 0;
		vertexBufDesc.StructureByteStride = 0;
		vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = points;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

		int indeces[] = { 0,1,2, 1,0,3 };
		D3D11_BUFFER_DESC indexBufDesc = {};
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indeces;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		device->CreateBuffer(&indexBufDesc, &indexData, &ib);

		CD3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;

		res = device->CreateRasterizerState(&rastDesc, &rastState);
	}
};

