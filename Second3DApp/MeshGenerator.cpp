#include "MeshGenerator.h"

MeshGenerator* MeshGenerator::generatorInstance = nullptr;

TexturedMesh MeshGenerator::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string dir)
{
	std::vector<Vertex> points;
	std::vector<int> indeces;

	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		DirectX::XMFLOAT4 point;
		point.x = mesh->mVertices[i].x;
		point.y = mesh->mVertices[i].y;
		point.z = mesh->mVertices[i].z;
		point.w = 1.0f;

		DirectX::XMFLOAT2 texCor = DirectX::XMFLOAT2(0.0f, 0.0f);
		if (mesh->mTextureCoords[0] != nullptr) {
			aiVector3D* pTexCoord = &(mesh->mTextureCoords[0][i]);
			texCor = DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y);
		};

		Vertex vertex = { point, texCor };
		points.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& Face = mesh->mFaces[i];
		indeces.push_back(Face.mIndices[0]);
		indeces.push_back(Face.mIndices[1]);
		indeces.push_back(Face.mIndices[2]);
		/*aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; j++)
			indeces.push_back(face.mIndices[j]);*/
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::cout << material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) << std::endl;
	aiString path;
	material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
	std::string filename = dir + std::string(path.C_Str());
	std::wstring texturePath = std::wstring (filename.begin(), filename.end());
	std::cout << filename << std::endl;

	TexturedMesh result = { points, indeces, texturePath };
	return result;
}

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

std::vector<TexturedMesh> MeshGenerator::getFromFile(const std::string& filepath)
{
	Assimp::Importer importer;

	std::string directory = "";
	size_t lastSlash = filepath.find_last_of("/\\");
	directory = filepath.substr(0, lastSlash + 1);

	std::vector<TexturedMesh> meshes;

	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_FixInfacingNormals);
	if (scene == nullptr) {
		std::cout << importer.GetErrorString() << std::endl;
		return meshes;
	}

	aiNode* node = scene->mRootNode;
	for (UINT i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		meshes.push_back(ProcessMesh(mesh, scene, directory));
	}

	return meshes;
}

