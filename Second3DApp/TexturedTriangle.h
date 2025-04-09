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

#include <DirectXTex.h> 

#include "GameComponent.h"
#include "ConstantBufferConfig.h";
#include "Vertex.h";
#include "Lighting.h";

class TexturedTriangle : public GameComponent
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

	ID3D11Buffer* constBuffer;
	ID3D11Buffer* lightBuffer;

	std::vector<UINT> strides;
	std::vector<UINT> offsets;

	std::vector<int> indeces;

	bool is2D = false;

	ID3D11ShaderResourceView* textureView;
	ID3D11SamplerState* samplerState;	
	
	ID3D11ShaderResourceView* shadowsResource;
	ID3D11SamplerState* shadowSampler;

	Material* material;
	
	//shadows
	ID3D11RasterizerState* rastState_shadows;

	ID3D11VertexShader* vertexShader_shadows;
	ID3DBlob* vertexByteCode_shadows;

	ID3D11PixelShader* pixelShader_shadows;
	ID3DBlob* pixelByteCode_shadows;

public:
	bool needMultip = true;
	std::vector<Vertex> points;

	Transformations transforms;
	ConstData constData;
	LightData lightData;

	TexturedTriangle(Game* gameInput) : GameComponent(gameInput)
	{
		layout = nullptr;

		vertexShader = nullptr;
		vertexByteCode = nullptr;
		vb = nullptr;

		pixelShader = nullptr;
		pixelByteCode = nullptr;

		ib = nullptr;
		rastState = nullptr;

		constBuffer = nullptr;
		lightBuffer = nullptr;

		//shadows
		vertexShader_shadows = nullptr;
		vertexByteCode_shadows = nullptr;
		pixelShader_shadows = nullptr;
		pixelByteCode_shadows = nullptr;

		shadowsResource = nullptr;
		shadowSampler = nullptr;
	};

	void Initialize(LPCWSTR shaderSource,
		std::vector<Vertex> pointsInput, std::vector<int> indecesInput,
		bool is2DInput, std::wstring texturePath, Material* materialInput);
	void CreateShadowShaders();

	void Draw();
	void LightRender();

	void Update();
	void LightUpdate();
	void DestroyResources();
};

