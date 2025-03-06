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
		DirectX::XMFLOAT4(0.04f, 0.04f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.04f, -0.04f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.04f, -0.04f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.04f, 0.04f, 1.0f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3 };

	Mesh result = { points, indeces };
	return result;
}

Mesh MeshGenerator::getVerticalRectangle(DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.025f, 0.15f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.025f, -0.15f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(0.025f, -0.15f, 1.0f, 1.0f), color,
		DirectX::XMFLOAT4(-0.025f, 0.15f, 1.0f, 1.0f), color,
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3 };

	Mesh result = { points, indeces };
	return result;
}

Mesh MeshGenerator::getCube(float size, DirectX::XMFLOAT4 color)
{
	std::vector<DirectX::XMFLOAT4> points = {
		// Front face
		DirectX::XMFLOAT4(-size, -size,  size, 1.0f), color,
		DirectX::XMFLOAT4(size, -size,  size, 1.0f), color,
		DirectX::XMFLOAT4(size,  size,  size, 1.0f), color,
		DirectX::XMFLOAT4(-size,  size,  size, 1.0f), color,

		// Back face
		DirectX::XMFLOAT4(-size, -size, -size, 1.0f), color,
		DirectX::XMFLOAT4(size, -size, -size, 1.0f), color,
		DirectX::XMFLOAT4(size,  size, -size, 1.0f), color,
		DirectX::XMFLOAT4(-size,  size, -size, 1.0f), color,
	};

	std::vector<int> indices = {	
		0, 1, 2,  0, 2, 3, //front face	
		4, 6, 5,  4, 7, 6, //back face
		4, 0, 3,  4, 3, 7, //left face
		1, 5, 6,  1, 6, 2, //right face
		3, 2, 6,  3, 6, 7, //top face
		4, 5, 1,  4, 1, 0 //bottom face
	};

	Mesh result = { points, indices };
	return result;
}

Mesh MeshGenerator::getSphere(float radius, int latitudeBands, int longitudeBands, DirectX::XMFLOAT4 color_first, DirectX::XMFLOAT4 color_second)
{
	std::vector<DirectX::XMFLOAT4> points;
	std::vector<int> indices;

	for (int lat = 0; lat <= latitudeBands; ++lat)
	{
		float theta = lat * DirectX::XM_PI / latitudeBands;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon <= longitudeBands; ++lon)
		{
			float phi = lon * 2 * DirectX::XM_PI / longitudeBands;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			float x = radius * cosPhi * sinTheta;
			float y = radius * cosTheta;
			float z = radius * sinPhi * sinTheta;

			points.push_back(DirectX::XMFLOAT4(x, y, z, 1.0f));
			DirectX::XMFLOAT4 color;
			if (lon % 2 == 0) color = color_first;
			else color = color_second;
			points.push_back(color);
		}
	}

	for (int lat = 0; lat < latitudeBands; ++lat)
	{
		for (int lon = 0; lon < longitudeBands; ++lon)
		{
			int first = lat * (longitudeBands + 1) + lon;
			int second = first + longitudeBands + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	Mesh result = { points, indices };
	return result;
}

