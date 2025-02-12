#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "TriangleComponent.h"

class GameComponent
{
private:
	TriangleComponent triangles;

public:
	GameComponent() {
		triangles = TriangleComponent();
	};

	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device);

	void Draw(ID3D11DeviceContext* context);

	void DestroyResources();
	void Reload();
	void Update();

};

