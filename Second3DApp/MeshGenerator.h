#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>

struct Mesh {
	std::vector<DirectX::XMFLOAT4> points;
	std::vector<int> indeces;
};

class MeshGenerator
{
private:
	static MeshGenerator* generatorInstance;
	MeshGenerator() {};

public:

	static MeshGenerator* getInstance() {
		if (generatorInstance == nullptr) {
			if (generatorInstance == nullptr) {
				generatorInstance = new MeshGenerator();
			}
		}
		return generatorInstance;
	}

	Mesh getSquare(DirectX::XMFLOAT4 color);
	Mesh getStar(DirectX::XMFLOAT4 color);
	Mesh getSmallSquare(DirectX::XMFLOAT4 color);
	Mesh getVerticalRectangle(DirectX::XMFLOAT4 color);

	Mesh getCube(DirectX::XMFLOAT4 color);
};

