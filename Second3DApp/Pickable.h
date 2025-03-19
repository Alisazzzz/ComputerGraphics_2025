#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

struct Pickable {
	std::vector<TexturedTriangle*> mesh;
	//DirectX::BoundingBox collision;
	DirectX::BoundingOrientedBox collision;
	Vector3 position;
	Vector3 rotation;
};
