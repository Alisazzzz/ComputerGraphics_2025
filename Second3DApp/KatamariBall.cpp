#include "KatamariBall.h"

#include "Game.h"
#include "Katamari.h"

float KatamariBall::Inertia(float speed, bool moving)
{
	float resultSpeed = speed;
	static float speedGrow = 0.04f;

	if (moving && resultSpeed < maxSpeed) { resultSpeed += speedGrow; }
	else if (moving && resultSpeed >= maxSpeed) { resultSpeed = maxSpeed; }
	else if (!moving && resultSpeed > 0.0f) { resultSpeed -= speedGrow; }
	else if (!moving && resultSpeed <= 0.0f) { resultSpeed = 0.0f; };

	return resultSpeed;
}

void KatamariBall::Jump(float deltaTime)
{
	if (a > 0.0f && !isFalling) {
		a -= 0.6 * deltaTime;
		jumpHeight -= a;
	}
	else if (a <= 0.0f && !isFalling) {
		isFalling = true;
	}

	if (isFalling) {
		a += 0.6 * deltaTime;
		jumpHeight += a;
		if (a >= heightMax) {
			jumpHeight = 0.0f;
			isFalling = false;
			isJumping = false;
		}
	}
}

void KatamariBall::AddingLights()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> distTheta(0, 2 * DirectX::XM_PI);
	std::uniform_real_distribution<> distPhi(0, DirectX::XM_PI);
	std::uniform_real_distribution<> distColor(0.0f, 2.0f);

	for (int i = 0; i < 8; ++i) {
		float theta = distTheta(gen);
		float phi = distPhi(gen);
		float R = radius + 1.0f;
		Vector3 lightPosition = Vector3(R * sin(phi) * cos(theta), R * sin(phi) * sin(theta), R * cos(phi));
		
		float colorR = distColor(gen);
		float colorG = distColor(gen);
		float colorB = distColor(gen);
		Vector4 lightAmbient = Vector4(colorR, colorG, colorB, 1.0f);
		Vector4 lightDiffuse = Vector4(colorR, colorG, colorB, 1.0f);
		Vector4 lightSpecular = Vector4(colorR, colorG, colorB, 4.0f);

		PointLight* point = new PointLight{
			lightAmbient,
			lightDiffuse,
			lightSpecular,
			lightPosition,
			1.3f,
			Vector4(0.6f, 0.9f, 1.0f, 1.0f),
		};
		lightsOrbits.push_back(lightPosition);
		game->pntLights.push_back(point);
	}
}

void KatamariBall::MoveKatamari(float deltaTime)
{
	Vector3 forward = mainOrbit->target - mainOrbit->lookPoint;
	forward.y = 0.0f;
	forward.Normalize();

	Vector3 right = forward.Cross(Vector3(0.0f, 1.0f, 0.0f));
	right.Normalize();
	
	Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);

	if (game->inputDevice->IsKeyDown(Keys::W)) { moveDirection += forward; movingForward = true; }
	if (game->inputDevice->IsKeyDown(Keys::S)) { moveDirection -= forward; movingForward = false; }

	//Jump
	if (game->inputDevice->IsKeyDown(Keys::Space)) { 
		if (!isJumping) {
			isJumping = true;
		}
	}

	if (isJumping) {
		Jump(deltaTime);
		position.y = -1.0f + jumpHeight;
	}

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

	/*katamariMesh = new TriangleComponent(game);
	Mesh ballSphere = MeshGenerator::getInstance()->getSphere(1.0f, 24, 24, Vector4(0.8f, 0.05f, 0.1f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	katamariMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", ballSphere.points, ballSphere.indeces, strides, offsets, false);
	game->components.push_back(katamariMesh);*/

	Material* mat = new Material{
		Vector4(0.23f, 0.23f, 0.23f, 1.00f),
		Vector4(0.28f, 0.28f, 0.28f, 1.00f),
		Vector4(0.77f, 0.77f, 0.77f, 5.9f)
	};

	katamariMesh = new TexturedTriangle(game);
	TexturedMesh ballSphere = MeshGenerator::getInstance()->getTexturedSphere(radius, 24, 24);
	katamariMesh->Initialize(L"./Shaders/MySecondShader.hlsl", ballSphere.points, ballSphere.indeces, false, L"./Textures/Earth_texture.jpeg", mat);
	game->components.push_back(katamariMesh);
	katamariMesh->transforms.move = Matrix::CreateTranslation(position);

	DirectX::BoundingSphere collision = DirectX::BoundingSphere(position, radius);

	Vector3 orbit = Vector3(2.0f, 2.0f, 2.0f);
	mainOrbit = new OrbitCamera(game);
	mainOrbit->Initialize(orbit, position - orbit, position);
	game->activeCamera = mainOrbit;
	game->components.push_back(mainOrbit);

	AddingLights();
}

void KatamariBall::CollisionCheck()
{
	for (Pickable* object : katamariGame->pickables) {
		if (collision.Intersects(object->collision) && !object->collected) {			
			collected.push_back(object);
			object->collected = true;
	
			Quaternion rotationInverted;
			rotation.Inverse(rotationInverted);	
			Quaternion localRotation = object->rotation * rotationInverted;
			object->rotation = localRotation;

			object->position = Vector3::Transform(object->position - position, rotationInverted);
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

		Vector3 newPosition = Vector3::Transform(object->position, rotation);

		for (TexturedTriangle* part : object->mesh) {			
			part->transforms.move = Matrix::CreateTranslation(newPosition + position);
			part->transforms.rotate = Matrix::CreateFromQuaternion(object->rotation * rotation);
		}
	}

	for (int i = 0; i < game->pntLights.size(); i++) {
		game->pntLights[i]->position = position + Vector3::Transform(lightsOrbits[i], rotation);
	}
}


OrbitCamera* KatamariBall::getCamera()
{
	return mainOrbit;
}
