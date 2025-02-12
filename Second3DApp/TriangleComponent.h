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
	ID3D11InputLayout* layout;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexByteCode;
	ID3D11Buffer* vb;

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelByteCode;

	ID3D11Buffer* ib;
	ID3D11RasterizerState* rastState;

public:

	TriangleComponent() {
		layout = nullptr;

		vertexShader = nullptr;
		vertexByteCode = nullptr;
		vb = nullptr;

		pixelShader = nullptr;
		pixelByteCode = nullptr;

		ib = nullptr;
		rastState = nullptr;
	};

	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device);

	void Draw(ID3D11DeviceContext* context);
};

