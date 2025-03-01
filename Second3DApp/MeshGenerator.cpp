#include "MeshGenerator.h"

MeshGenerator* MeshGenerator::generatorInstance = nullptr;

Mesh MeshGenerator::getSquare(DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3 };

	Mesh result = { points, indeces };
	return result;
}
Mesh MeshGenerator::getStar(DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.15f, 0.25f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.6f, 0.25f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.22f, -0.0f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.365f, -0.45f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.0f, -0.15f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.365f, -0.45f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.22f, -0.0f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.6f, 0.25f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.15f, 0.25f, 1.0f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,9, 1,2,3, 3,4,5, 5,6,7, 7,8,9, 1,5,9, 1,3,5, 9,5,7 };

	Mesh result = { points, indeces };
	return result;
}
Mesh MeshGenerator::getSmallSquare(DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.04f, 0.04f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.04f, -0.04f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(0.04f, -0.04f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.04f, 0.04f, 0.5f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3 };

	Mesh result = { points, indeces };
	return result;
}
Mesh MeshGenerator::getVerticalRectangle(DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.025f, 0.15f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.025f, -0.15f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(0.025f, -0.15f, 0.5f, 1.0f), color,
		DirectX::XMFLOAT4(-0.025f, 0.15f, 0.5f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3 };

	Mesh result = { points, indeces };
	return result;
}
;
