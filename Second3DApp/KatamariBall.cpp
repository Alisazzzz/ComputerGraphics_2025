#include "KatamariBall.h"

#include "Game.h"
#include "Katamari.h"

KatamariBall::KatamariBall(Game* gameInput)
{
	game = gameInput;
	katamariGame = Katamari::getInstance();

	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };	

	katamariMesh = new TriangleComponent(game);
	Mesh ballSphere = MeshGenerator::getInstance()->getSphere(1.0f, 24, 24, Vector4(0.8f, 0.05f, 0.1f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	katamariMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", ballSphere.points, ballSphere.indeces, strides, offsets, false);
	game->components.push_back(katamariMesh);

	DirectX::BoundingSphere collision = DirectX::BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), 1.0f);

	Vector3 orbit = Vector3(2.0f, 2.0f, 2.0f);
	mainOrbit = new OrbitCamera(game);
	mainOrbit->Initialize(orbit, position - orbit, position);
	game->activeCamera = mainOrbit;
	game->components.push_back(mainOrbit);
}

void KatamariBall::CollisionCheck()
{
	for (Pickable* object : katamariGame->pickables) {
		if (collision.Intersects(object->collision)) {
			std::cout << "yesss" << std::endl;
		}
	}
}

void KatamariBall::UpdateInterval(float deltaTime)
{
	Vector3 forward = mainOrbit->target - mainOrbit->lookPoint;
	forward.Normalize();
	forward.y = 0.0f;

	Vector3 right = mainOrbit->upAxis.Cross(forward);
	right.Normalize();
	right.y = 0.0f;

	Vector3 rotationAxis(0.0f, 0.0f, 0.0f);
	float rotationAngle = 0.0f;

	if (game->inputDevice->IsKeyDown(Keys::W)) {
		velocity = forward * speed * deltaTime;
		rotationAxis = right;
		rotationAngle = (speed * deltaTime) / radius;
	}
	if (game->inputDevice->IsKeyDown(Keys::S)) {
		velocity = -forward * speed * deltaTime;
		rotationAxis = -right;
		rotationAngle = (speed * deltaTime) / radius;
	}
	if (game->inputDevice->IsKeyDown(Keys::A)) {
		velocity = -right * speed * deltaTime;
		rotationAxis = forward;
		rotationAngle = (speed * deltaTime) / radius;
	}
	if (game->inputDevice->IsKeyDown(Keys::D)) {
		velocity = right * speed * deltaTime;
		rotationAxis = -forward;
		rotationAngle = (speed * deltaTime) / radius;
	}

	if (!(game->inputDevice->IsKeyDown(Keys::W)) &&
		!(game->inputDevice->IsKeyDown(Keys::S)) &&
		!(game->inputDevice->IsKeyDown(Keys::A)) &&
		!(game->inputDevice->IsKeyDown(Keys::D)))
		velocity = Vector3(0.0f, 0.0f, 0.0f);

	position += velocity;
	if (rotationAngle > 0.0f) {
		rotation += rotationAxis * rotationAngle;
	}

	CollisionCheck();
}

void KatamariBall::Update()
{
	collision.Center = position;
	katamariMesh->transforms.move = Matrix::CreateTranslation(position);
	katamariMesh->transforms.rotate = Matrix::CreateFromYawPitchRoll(rotation);
	mainOrbit->SetTarget(position);
	mainOrbit->SetLookPoint(position + Vector3::Transform(mainOrbit->orbit, Matrix::CreateRotationY(0.0f)));
}


OrbitCamera* KatamariBall::getCamera()
{
	return mainOrbit;
}
