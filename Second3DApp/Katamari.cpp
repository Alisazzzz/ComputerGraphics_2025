#include "Katamari.h"
#include "Game.h"

Katamari* Katamari::katamariInstance = nullptr;

using namespace std; 

void Katamari::Initialize()
{
	game = Game::getInstance();

	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

	RandomObjectGeneration();

	ball = new KatamariBall(game);
	mainOrbit = ball->getCamera();

	light = new DiffuseLight(game);
	light->Initialize();
	light->lightColor = Vector3(1.0f, 1.0f, 1.0f);
	light->lightStrength = 1.0f;
	light->position = Vector4(0.0f, -3.0f, 0.0f, 1.0f);
	game->light = light;
}

void Katamari::RandomObjectGeneration()
{
	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<> distX(ldMapCorner.x, ruMapCorner.x);
	uniform_real_distribution<> distZ(ldMapCorner.z, ruMapCorner.z);
	uniform_real_distribution<> rotY(0, DirectX::XM_2PI);

	std::vector<std::vector<TexturedMesh>> models;
	models.push_back(MeshGenerator::getInstance()->getFromFile("./Models/Rose/Red_rose_SF.obj"));
	models.push_back(MeshGenerator::getInstance()->getFromFile("./Models/PinkRose/Pink_rose_retopo_SF.obj"));

	uniform_int_distribution<> modelDist(0, models.size()-1);

	for (int i = 0; i < objectsCount; i++) {
		Vector3 position = Vector3(distX(gen), 0.0f, distZ(gen));
		float rotationY = rotY(gen);
		std::cout << rotationY << std::endl;

		std::vector<TexturedMesh> meshes = models.at(modelDist(gen));
		std::vector<TexturedTriangle*> modelParts;

		for (TexturedMesh mesh : meshes) {
			TexturedTriangle* modelPart = new TexturedTriangle(game);
			modelPart->Initialize(L"./Shaders/MySecondShader.hlsl", mesh.points, mesh.indeces, false, mesh.texturePath);
			modelPart->transforms.rotate = Matrix::CreateFromYawPitchRoll(Vector3(DirectX::XM_PIDIV2, DirectX::XM_PIDIV2, rotationY));
			modelPart->transforms.move = Matrix::CreateTranslation(position);
			game->components.push_back(modelPart);
			modelParts.push_back(modelPart);
		}

		DirectX::BoundingOrientedBox collision;
		collision.Extents = Vector3(0.05f, 1.1f, 0.05f);
;		collision.Orientation = Quaternion::CreateFromYawPitchRoll(Vector3(DirectX::XM_PIDIV2, DirectX::XM_PIDIV2, rotationY));
		collision.Center = Vector3(position.x, position.y, position.z);
		
		Pickable* object = new Pickable{
			modelParts,
			collision,
			position,
			Quaternion::CreateFromYawPitchRoll(DirectX::XM_PIDIV2, DirectX::XM_PIDIV2, rotationY),
			false,
		};

		pickables.push_back(object);
	}
}

void Katamari::Update()
{
	ball->Update();
}

void Katamari::UpdateInterval(float deltaTime)
{
	ball->UpdateInterval(deltaTime);
}
