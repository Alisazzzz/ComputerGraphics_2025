#include "KatamariBall.h"

#include "Game.h"
#include "Katamari.h"

float KatamariBall::Inertia(float speed, bool moving)
{
	float resultSpeed = speed;
	static float speedGrow = 0.02f;

	if (moving && resultSpeed < maxSpeed) { resultSpeed += speedGrow; }
	else if (moving && resultSpeed >= maxSpeed) { resultSpeed = maxSpeed; }
	else if (!moving && resultSpeed > 0.0f) { resultSpeed -= speedGrow; }
	else if (!moving && resultSpeed <= 0.0f) { resultSpeed = 0.0f; };

	return resultSpeed;
}

void KatamariBall::MoveKatamari(float deltaTime)
{
	Vector3 forward = mainOrbit->target - mainOrbit->lookPoint;
	forward.y = 0.0f;
	forward.Normalize();

	Vector3 right = forward.Cross(mainOrbit->upAxis);
	right.Normalize();
	
	Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);

	if (game->inputDevice->IsKeyDown(Keys::W)) { moveDirection += forward; movingForward = true; }
	if (game->inputDevice->IsKeyDown(Keys::S)) { moveDirection -= forward; movingForward = false; }

	if ((moveDirection.x != 0) || (moveDirection.y != 0) || (moveDirection.z != 0)) {
		moveDirection.Normalize();
		velocity = moveDirection;
		speed = Inertia(speed, true);
		position += moveDirection * speed * deltaTime;	
	}
	else {
		speed = Inertia(speed, false);
		position += velocity * speed * deltaTime;
	}
	
	float rotationAngle = speed * deltaTime / radius;
	Quaternion currentDelta;

	if (movingForward) { currentDelta = Quaternion::CreateFromAxisAngle(right, rotationAngle); }	
	else { currentDelta = Quaternion::CreateFromAxisAngle(right, -rotationAngle); }	

	rotationDelta = Matrix::CreateFromQuaternion(currentDelta);

	Quaternion newRotation = rotation * currentDelta;
	newRotation.Normalize();
	rotation = newRotation;
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
	TexturedMesh ballSphere = MeshGenerator::getInstance()->getTexturedSphere(radius, 24, 24);
	katamariMesh->Initialize(L"./Shaders/MySecondShader.hlsl", ballSphere.points, ballSphere.indeces, strides, offsets, false, L"./Textures/Earth_texture.jpeg");
	game->components.push_back(katamariMesh);

	DirectX::BoundingSphere collision = DirectX::BoundingSphere(position, radius);

	Vector3 orbit = Vector3(2.0f, 2.0f, 2.0f);
	mainOrbit = new OrbitCamera(game);
	mainOrbit->Initialize(orbit, position - orbit, position);
	game->activeCamera = mainOrbit;
	game->components.push_back(mainOrbit);
}

void KatamariBall::CollisionCheck()
{
	for (Pickable* object : katamariGame->pickables) {
		if (collision.Intersects(object->collision) && !object->collected) {			
			collected.push_back(object);
			object->collected = true;
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
	katamariMesh->transforms.rotate = Matrix::CreateFromQuaternion(rotation);

	mainOrbit->SetTarget(position);
	mainOrbit->SetLookPoint(position + mainOrbit->orbit);

	for (Pickable* object : collected) {

		Vector3 localPosition = object->position - position;
		localPosition = Vector3::Transform(localPosition, rotationDelta);
		object->position = localPosition + position;

		object->rotation *= Quaternion::CreateFromRotationMatrix(rotationDelta);

		for (TexturedTriangle* part : object->mesh) {			
			part->transforms.move = Matrix::CreateTranslation(object->position);
			part->transforms.rotate = Matrix::CreateFromQuaternion(object->rotation);			 						
		}
	}
}


OrbitCamera* KatamariBall::getCamera()
{
	return mainOrbit;
}
