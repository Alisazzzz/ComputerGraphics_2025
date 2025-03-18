#include "Katamari.h"
#include "Game.h"

Katamari* Katamari::katamariInstance = nullptr;

void Katamari::Initialize()
{
	game = Game::getInstance();

	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

	mainFPS = new FPSCamera(game);
	mainFPS->Initialize();
	game->activeCamera = mainFPS;
	game->components.push_back(mainFPS);

	std::vector<DirectX::XMFLOAT4> lines;

	for (int i = 0; i < 200; i++) {
		lines.push_back(Vector4(-150.0f, 0.0f, -50.0f + i, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		lines.push_back(Vector4(150.0f, 0.0f, -50.0f + i, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	for (int i = 0; i < 200; i++) {
		lines.push_back(Vector4(-50.0f + i, 0.0f, -150.0f, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		lines.push_back(Vector4(-50.0f + i, 0.0f, 150.0f, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	LinelistComponent* linesTriangle1 = new LinelistComponent(game);
	linesTriangle1->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	game->components.push_back(linesTriangle1);

	LinelistComponent* linesTriangle2 = new LinelistComponent(game);
	linesTriangle2->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	linesTriangle2->transforms.rotate = Matrix::CreateRotationY(DirectX::XM_PIDIV2);
	game->components.push_back(linesTriangle2);

	strides = { 24 };
	offsets = { 0 };

	std::vector<TexturedMesh> sword = MeshGenerator::getInstance()->getFromFile("./Models/SlothSword/sword.obj");
	for (TexturedMesh mesh : sword) {
		TexturedTriangle* swordPart = new TexturedTriangle(game);
		swordPart->Initialize(L"./Shaders/MySecondShader.hlsl", mesh.points, mesh.indeces, strides, offsets, false, mesh.texturePath);
		//swordPart->transforms.scale = Matrix::CreateScale(Vector3(0.5f, 0.5f, 0.5f));
		//swordPart->transforms.rotate = Matrix::CreateRotationX(DirectX::XM_PIDIV2);
		game->components.push_back(swordPart);
	}
}

void Katamari::RandomObjectGeneration()
{
	std::srand(std::time(0));
	std::vector<LPCWSTR> models;
	models.push_back(L"./Models/Rose/Red_rose_SF.obj");
	models.push_back(L"./Models/SlothSword/sword.obj");

	for (int)
}

void Katamari::UpdateInterval()
{

}
