#pragma once

#include <wrl.h>
#include <iostream>
#include <algorithm>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "GameComponent.h"

class TriangleComponent : public GameComponent
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

	std::vector<DirectX::XMFLOAT4> points;

	const UINT* strides;
	std::vector<UINT> offsets;

public:
	TriangleComponent(Game* gameInput) : GameComponent(gameInput) 
	{
		layout = nullptr;

		vertexShader = nullptr;
		vertexByteCode = nullptr;
		vb = nullptr;

		pixelShader = nullptr;
		pixelByteCode = nullptr;

		ib = nullptr;
		rastState = nullptr;
	};

	void Initialize(LPCWSTR shaderSource, std::vector<DirectX::XMFLOAT4> pointsInput, std::vector<UINT> stridesInput, std::vector<UINT> offsetsInput);

	void Draw();
	void Update();

	void DestroyResources();
};

