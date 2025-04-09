#pragma once

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <SimpleMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX::SimpleMath;

#include "Lighting.h";

struct ConstData {
	Matrix transformations;
	Matrix view;
	Matrix projection;
	Vector4 color;
};

struct Transformations {
	Matrix move;
	Matrix rotate;
	Matrix scale;
};

struct Material {
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};

struct LightData {	
	Material material;
	DirectionalLight directional;
	PointLight points[8];
	Vector4 spectator;
	Matrix lightSpace;
};
