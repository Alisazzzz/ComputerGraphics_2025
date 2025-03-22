#include "KatamariBall.h"

#include "Game.h"
#include "Katamari.h"

void KatamariBall::MoveKatamari(float deltaTime)
{
	Vector3 forward = mainOrbit->target - mainOrbit->lookPoint;
	forward.y = 0.0f;
	forward.Normalize();

	Vector3 right = mainOrbit->upAxis.Cross(forward);
	right.y = 0.0f;
	right.Normalize();
	
	Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);

	float rotationSpeed = speed / radius;
	float rotationAngle = rotationSpeed * deltaTime;

	if (game->inputDevice->IsKeyDown(Keys::W)) {
		moveDirection += forward;
		rotation.x -= rotationAngle;
	}
	if (game->inputDevice->IsKeyDown(Keys::S)) {
		moveDirection -= forward;
		rotation.x += rotationAngle;
	}
	//if (game->inputDevice->IsKeyDown(Keys::A)) {
	//	moveDirection -= right;
	//	rotation.z += rotationAngle;
	//}
	//if (game->inputDevice->IsKeyDown(Keys::D)) {
	//	moveDirection += right;
	//	rotation.z -= rotationAngle;
	//}
	 
	if ((moveDirection.x != 0) || (moveDirection.y != 0) || (moveDirection.z != 0)) {
		velocity = moveDirection;
		moveDirection.Normalize();
		position += moveDirection * speed * deltaTime;
		if (speed < 3.0f)
			speed += 0.02f;
	}
	else if (speed > 0.0f) {
		speed -= 0.02f;
		rotationSpeed = speed / radius;
		rotationAngle = rotationSpeed * deltaTime;

		position += velocity * speed * deltaTime;
		if (velocity == -forward) {
			rotation.x += rotationAngle;
		}
		else if (velocity == forward) {
			rotation.x -= rotationAngle;
		}
	};

	katamariMesh->Update();
	if ((moveDirection.x != 0) || (moveDirection.y != 0) || (moveDirection.z != 0))
		rotation.y = atan2(forward.x, forward.z);
}

KatamariBall::KatamariBall(Game* gameInput)
{
	game = gameInput;
	katamariGame = Katamari::getInstance();

	std::vector<UINT> strides = { 24 };
	std::vector<UINT> offsets = { 0 };	

	/*katamariMesh = new TriangleComponent(game);
	Mesh ballSphere = MeshGenerator::getInstance()->getSphere(1.0f, 24, 24, Vector4(0.8f, 0.05f, 0.1f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	katamariMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", ballSphere.points, ballSphere.indeces, strides, offsets, false);
	game->components.push_back(katamariMesh);*/

	katamariMesh = new TexturedTriangle(game);
	TexturedMesh ballSphere = MeshGenerator::getInstance()->getTexturedSphere(1.0f, 24, 24);
	katamariMesh->Initialize(L"./Shaders/MySecondShader.hlsl", ballSphere.points, ballSphere.indeces, strides, offsets, false, L"./Textures/Earth_texture.jpeg");
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
			Vector3 vectorToObject = collision.Center - object->collision.Center;
			//vectorToObject.Normalize();
			//vectorToObject *= radius;
			object->position = vectorToObject;
			//Matrix ballInverted = katamariMesh->constData.transformations.Invert();
			//object->tranformations *= ballInverted;
			collected.push_back(object);
		}
	}
}

void KatamariBall::UpdateInterval(float deltaTime)
{
	MoveKatamari(deltaTime);
	CollisionCheck();
}

void KatamariBall::Update()
{
	collision.Center = position;
	katamariMesh->transforms.move = Matrix::CreateTranslation(position);
	katamariMesh->transforms.rotate = Matrix::CreateFromYawPitchRoll(rotation);
	mainOrbit->SetTarget(position);
	mainOrbit->SetLookPoint(position + Vector3::Transform(mainOrbit->orbit, Matrix::CreateRotationY(0.0f)));

	for (Pickable* object : collected) {
		Vector3 rotationPart = object->rotation;
		Vector3 positionPart = object->position;
		for (TexturedTriangle* part : object->mesh) {
			//part->transforms.rotate = Matrix::CreateFromYawPitchRoll(object->rotation) * Matrix::CreateFromYawPitchRoll(rotation);

			//Vector3 localPos = object->position - position;
			//localPos = Vector3::Transform(localPos, Matrix::CreateFromYawPitchRoll(rotation));
			//position = position + localPos;
	
			part->transforms.rotate = Matrix::CreateFromYawPitchRoll(rotationPart) * katamariMesh->transforms.rotate;
			Vector3 newObjectPosition = Vector3::Transform(object->position, Matrix::CreateFromYawPitchRoll(rotation));
			part->transforms.move = Matrix::CreateTranslation(positionPart + newObjectPosition) * katamariMesh->transforms.move;
			//part->transforms.move = Matrix::CreateTranslation(position + newObjectPosition);
			//part->needMultip = false;
			//part->constData.transformations = (katamariMesh->transforms.scale * katamariMesh->transforms.rotate * katamariMesh->transforms.move) * object->tranformations;
			//part->transforms.move = Matrix::CreateTranslation(position);			
		}
	}
}


OrbitCamera* KatamariBall::getCamera()
{
	return mainOrbit;
}
