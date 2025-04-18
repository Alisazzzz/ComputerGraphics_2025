#include "Katamari.h"
#include "Game.h"

Katamari* Katamari::katamariInstance = nullptr;

using namespace std; 

void Katamari::Initialize()
{
	game = Game::getInstance();

	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

	TexturedTriangle* floor = new TexturedTriangle(game);
	Material* floorMaterial = new Material{
		Vector4(0.2f, 0.2f, 0.2f, 0.2f),
		Vector4(0.2f, 0.2f, 0.2f, 0.2f),
		Vector4(0.0f, 0.1f, 0.0f, 1.00f)
	};

	TexturedMesh floorMesh = MeshGenerator::getInstance()->getFromFile("./Models/Floor/floor.obj")[0];
	floor->Initialize(L"./Shaders/MySecondShader.hlsl", floorMesh.points, floorMesh.indeces, false, floorMesh.texturePath, floorMaterial);
	floor->transforms.move = Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f));
	game->components.push_back(floor);
	game->meshes.push_back(floor);

	RandomObjectGeneration();

	ball = new KatamariBall(game);
	mainOrbit = ball->getCamera();

	light = new DirectionalLight{
		Vector4(0.9f, 0.9f, 0.9f, 1.0f),
		Vector4(1.0f, 1.1f, 1.1f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 0.1f),
		Vector4(0.0f, 1.0f, 1.0f, 1.0f)	
	};

	DirectionalLight* lightNull = new DirectionalLight{
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4(0.0f, 0.0f, 0.0f, 0.01f),
		Vector4(0.0f, 1.0f, 1.0f, 1.0f)
	};

	game->dirLight = light;
}

void Katamari::RandomObjectGeneration()
{
	random_device rd;
	mt19937 gen(rd());

	uniform_real_distribution<> distX(ldMapCorner.x, ruMapCorner.x);
	uniform_real_distribution<> distZ(ldMapCorner.z, ruMapCorner.z);
	uniform_real_distribution<> rotY(0, DirectX::XM_2PI);

	std::vector<std::vector<TexturedMesh>> models;
	std::vector<Vector3> extents;

	models.push_back(MeshGenerator::getInstance()->getFromFile("./Models/Rose/Red_rose_SF.obj"));
	extents.push_back(Vector3(0.05f, 1.1f, 0.05f));

	models.push_back(MeshGenerator::getInstance()->getFromFile("./Models/PinkRose/Pink_rose_retopo_SF.obj"));
	extents.push_back(Vector3(0.2f, 0.2f, 0.2f));

	uniform_int_distribution<> modelDist(0, models.size()-1);

	for (int i = 0; i < objectsCount; i++) {
		Vector3 position = Vector3(distX(gen), -0.25f, distZ(gen));
		float rotationY = rotY(gen);
		std::cout << rotationY << std::endl;

		int currentObject = modelDist(gen);
		std::vector<TexturedMesh> meshes = models.at(currentObject);
		std::vector<TexturedTriangle*> modelParts;

		for (TexturedMesh mesh : meshes) {
			TexturedTriangle* modelPart = new TexturedTriangle(game);

			Material* mat = new Material {
				Vector4(0.2f, 0.2f, 0.2f, 0.2f),
				Vector4(0.2f, 0.2f, 0.2f, 0.2f),
				Vector4(0.55f, 0.55f, 0.55f, 1.00f)
			};
			modelPart->Initialize(L"./Shaders/MySecondShader.hlsl", mesh.points, mesh.indeces, false, mesh.texturePath, mat);
			modelPart->transforms.rotate = Matrix::CreateFromYawPitchRoll(Vector3(DirectX::XM_PIDIV2, DirectX::XM_PIDIV2, rotationY));
			modelPart->transforms.move = Matrix::CreateTranslation(position);
			game->components.push_back(modelPart);
			game->meshes.push_back(modelPart);
			modelParts.push_back(modelPart);
		}

		DirectX::BoundingOrientedBox collision;
		collision.Extents = extents.at(currentObject);
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
