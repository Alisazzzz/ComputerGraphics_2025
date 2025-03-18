#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>

#include "Vertex.h";

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

struct Mesh {
	std::vector<DirectX::XMFLOAT4> points;
	std::vector<int> indeces;
};

struct TexturedMesh {
	std::vector<Vertex> points;
	std::vector<int> indeces;
	std::wstring texturePath;
};

class MeshGenerator
{
private:
	static MeshGenerator* generatorInstance;
	MeshGenerator() {};

	TexturedMesh ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string dir);

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

	Mesh getCube(float size, DirectX::XMFLOAT4 color);
	Mesh getSphere(float radius, int latitudeBands, int longitudeBands, 
		DirectX::XMFLOAT4 color_first, DirectX::XMFLOAT4 color_second);

	std::vector<TexturedMesh> getFromFile(const std::string& filepath);
};

