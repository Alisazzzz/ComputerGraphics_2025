#include "PlanetSystem.h"
#include "Game.h"

#include <iostream>

PlanetSystem* PlanetSystem::planetSystemInstance = nullptr;

void PlanetSystem::Initialize()
{
	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

	game = Game::getInstance();

	Vector3 camOrbit = Vector3(-2.0f, -1.0f, -2.0f);

	//Planet system center
	PlanetObject* center = new PlanetObject{
		nullptr,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f,0.0f, 0.0f),
		nullptr
	};

	//Sun
	TriangleComponent* sunMesh = new TriangleComponent(game);
	Mesh sunCube = MeshGenerator::getInstance()->getCube(1.0, Vector4(1.0f, 0.8f, 0.0f, 1.0f));
	sunMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", sunCube.points, sunCube.indeces, strides, offsets, false);
	game->components.push_back(sunMesh);

	PlanetObject* sun = new PlanetObject{
		sunMesh,
		0.0f,
		0.5f,
		0.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		center
	};
	objects.push_back(sun);

	OrbitCamera* sunCam = new OrbitCamera(game);
	sunCam->Initialize(camOrbit, sun->position - camOrbit, sun->position);
	OrbitCameraObject* sunCamObject = new OrbitCameraObject{ sunCam, sun };
	cameras.push_back(sunCamObject);
	game->components.push_back(sunCam);


	//Mercury
	TriangleComponent* mercuryMesh = new TriangleComponent(game);
	Mesh mercurySphere = MeshGenerator::getInstance()->getSphere(0.2f, 24, 24, Vector4(0.8f, 0.05f, 0.1f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	mercuryMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", mercurySphere.points, mercurySphere.indeces, strides, offsets, false);
	game->components.push_back(mercuryMesh);

	PlanetObject* mercury = new PlanetObject{
		mercuryMesh,
		0.0f,
		0.6f,
		0.0f,
		0.5f,
		Vector3(2.0f, 0.0f, 0.0f),
		Vector3(2.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(mercury);
	mercury->mesh->transforms.move = Matrix::CreateTranslation(mercury->position);

	OrbitCamera* mercuryCam = new OrbitCamera(game);
	mercuryCam->Initialize(camOrbit, mercury->position - camOrbit, mercury->position);
	OrbitCameraObject* mercuryCamObject = new OrbitCameraObject{ mercuryCam, mercury };
	cameras.push_back(mercuryCamObject);
	game->components.push_back(mercuryCam);


	//Venus
	TriangleComponent* venusMesh = new TriangleComponent(game);
	Mesh venusSphere = MeshGenerator::getInstance()->getSphere(0.25f, 24, 24, Vector4(0.6f, 0.3f, 0.05f, 1.0f), Vector4(0.7f, 0.2f, 0.1f, 1.0f));
	venusMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", venusSphere.points, venusSphere.indeces, strides, offsets, false);
	game->components.push_back(venusMesh);

	PlanetObject* venus = new PlanetObject{
		venusMesh,
		0.0f,
		0.8f,
		0.0f,
		0.7f,
		Vector3(3.0f, 0.0f, 0.0f),
		Vector3(3.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(venus);
	venus->mesh->transforms.move = Matrix::CreateTranslation(venus->position);

	OrbitCamera* venusCam = new OrbitCamera(game);
	venusCam->Initialize(camOrbit, venus->position - camOrbit, venus->position);
	OrbitCameraObject* venusCamObject = new OrbitCameraObject{ venusCam, venus };
	cameras.push_back(venusCamObject);
	game->components.push_back(venusCam);


	//Earth
	TriangleComponent* earthMesh = new TriangleComponent(game);
	Mesh earthCube = MeshGenerator::getInstance()->getCube(0.1, Vector4(0.0f, 0.3f, 1.0f, 1.0f));
	earthMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", earthCube.points, earthCube.indeces, strides, offsets, false);
	game->components.push_back(earthMesh);

	PlanetObject* earth = new PlanetObject{
		earthMesh,
		0.0f,
		30.0f,
		0.0f,
		0.6f,
		Vector3(4.0f, 0.0f, 0.0f),
		Vector3(4.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(earth);
	earth->mesh->transforms.move = Matrix::CreateTranslation(earth->position);

	OrbitCamera* earthCam = new OrbitCamera(game);
	earthCam->Initialize(camOrbit, earth->position - camOrbit, earth->position);
	OrbitCameraObject* earthCamObject = new OrbitCameraObject{ earthCam, earth };
	cameras.push_back(earthCamObject);
	game->components.push_back(earthCam);

	//Lune
	TriangleComponent* luneMesh = new TriangleComponent(game);
	Mesh luneCube = MeshGenerator::getInstance()->getCube(0.05, Vector4(0.4f, 0.4f, 0.4f, 1.0f));
	luneMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", luneCube.points, luneCube.indeces, strides, offsets, false);
	game->components.push_back(luneMesh);

	PlanetObject* lune = new PlanetObject{
		luneMesh,
		0.0f,
		0.9f,
		0.0f,
		4.0f,
		Vector3(0.4f, 0.0f, 0.0f),
		Vector3(0.4f, 0.0f, 0.0f),
		earth
	};
	objects.push_back(lune);
	lune->mesh->transforms.move = Matrix::CreateTranslation(lune->position + lune->parent->position);

	//Mars
	TriangleComponent* marsMesh = new TriangleComponent(game);
	Mesh marsSphere = MeshGenerator::getInstance()->getSphere(0.15f, 24, 24, Vector4(0.3f, 0.0f, 0.0f, 1.0f), Vector4(0.5f, 0.0f, 0.0f, 1.0f));
	marsMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", marsSphere.points, marsSphere.indeces, strides, offsets, false);
	game->components.push_back(marsMesh);

	PlanetObject* mars = new PlanetObject{
		marsMesh,
		0.0f,
		0.2f,
		0.0f,
		0.7f,
		Vector3(5.0f, 0.0f, 0.0f),
		Vector3(5.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(mars);
	mars->mesh->transforms.move = Matrix::CreateTranslation(mars->position);

	OrbitCamera* marsCam = new OrbitCamera(game);
	marsCam->Initialize(camOrbit, mars->position - camOrbit, mars->position);
	OrbitCameraObject* marsCamObject = new OrbitCameraObject{ marsCam, mars };
	cameras.push_back(marsCamObject);
	game->components.push_back(marsCam);

	//Phobos
	TriangleComponent* phobosMesh = new TriangleComponent(game);
	Mesh phobosSphere = MeshGenerator::getInstance()->getSphere(0.05f, 24, 24, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	phobosMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", phobosSphere.points, phobosSphere.indeces, strides, offsets, false);
	game->components.push_back(phobosMesh);

	PlanetObject* phobos = new PlanetObject{
		phobosMesh,
		0.0f,
		0.9f,
		0.0f,
		0.8f,
		Vector3(0.25f, 0.0f, 0.0f),
		Vector3(0.25f, 0.0f, 0.0f),
		mars
	};
	objects.push_back(phobos);
	phobos->mesh->transforms.move = Matrix::CreateTranslation(phobos->position + phobos->parent->position);

	//Deimos
	TriangleComponent* deimosMesh = new TriangleComponent(game);
	Mesh deimosSphere = MeshGenerator::getInstance()->getSphere(0.06f, 24, 24, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(0.3f, 0.3f, 0.3f, 1.0f));
	deimosMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", deimosSphere.points, deimosSphere.indeces, strides, offsets, false);
	game->components.push_back(deimosMesh);

	PlanetObject* deimos = new PlanetObject{
		deimosMesh,
		0.0f,
		0.8f,
		0.0f,
		0.9f,
		Vector3(0.45f, 0.0f, 0.0f),
		Vector3(0.45f, 0.0f, 0.0f),
		mars
	};
	objects.push_back(deimos);
	deimos->mesh->transforms.move = Matrix::CreateTranslation(deimos->position + deimos->parent->position);

	//Jupiter
	TriangleComponent* jupiterMesh = new TriangleComponent(game);
	Mesh jupiterSphere = MeshGenerator::getInstance()->getSphere(0.5f, 24, 24, Vector4(0.6f, 0.3f, 0.05f, 1.0f), Vector4(0.7f, 0.4f, 0.0f, 1.0f));
	jupiterMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", jupiterSphere.points, jupiterSphere.indeces, strides, offsets, false);
	game->components.push_back(jupiterMesh);

	PlanetObject* jupiter = new PlanetObject{
		jupiterMesh,
		0.0f,
		0.5f,
		0.0f,
		0.4f,
		Vector3(7.0f, 0.0f, 0.0f),
		Vector3(7.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(jupiter);
	jupiter->mesh->transforms.move = Matrix::CreateTranslation(jupiter->position);

	OrbitCamera* jupiterCam = new OrbitCamera(game);
	jupiterCam->Initialize(camOrbit, jupiter->position - camOrbit, jupiter->position);
	OrbitCameraObject* jupiterCamObject = new OrbitCameraObject{ jupiterCam, jupiter };
	cameras.push_back(jupiterCamObject);
	game->components.push_back(jupiterCam);

	//Saturn
	TriangleComponent* saturnMesh = new TriangleComponent(game);
	Mesh saturnSphere = MeshGenerator::getInstance()->getSphere(0.4f, 24, 24, Vector4(0.6f, 0.5f, 0.05f, 1.0f), Vector4(0.7f, 0.6f, 0.0f, 1.0f));
	saturnMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", saturnSphere.points, saturnSphere.indeces, strides, offsets, false);
	game->components.push_back(saturnMesh);

	PlanetObject* saturn = new PlanetObject{
		saturnMesh,
		0.0f,
		0.9f,
		0.0f,
		0.6f,
		Vector3(9.0f, 0.0f, 0.0f),
		Vector3(9.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(saturn);
	saturn->mesh->transforms.move = Matrix::CreateTranslation(saturn->position);

	OrbitCamera* saturnCam = new OrbitCamera(game);
	saturnCam->Initialize(camOrbit, saturn->position - camOrbit, saturn->position);
	OrbitCameraObject* saturnCamObject = new OrbitCameraObject{ saturnCam, saturn };
	cameras.push_back(saturnCamObject);
	game->components.push_back(saturnCam);

	//Uranus
	TriangleComponent* uranusMesh = new TriangleComponent(game);
	Mesh uranusSphere = MeshGenerator::getInstance()->getSphere(0.35f, 24, 24, Vector4(0.0f, 0.5f, 0.8f, 1.0f), Vector4(0.0f, 0.6f, 1.0f, 1.0f));
	uranusMesh->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", uranusSphere.points, uranusSphere.indeces, strides, offsets, false);
	game->components.push_back(uranusMesh);

	PlanetObject* uranus = new PlanetObject{
		uranusMesh,
		0.0f,
		1.3f,
		0.0f,
		0.5f,
		Vector3(11.0f, 0.0f, 0.0f),
		Vector3(11.0f, 0.0f, 0.0f),
		sun
	};
	objects.push_back(uranus);
	uranus->mesh->transforms.move = Matrix::CreateTranslation(uranus->position);

	OrbitCamera* uranusCam = new OrbitCamera(game);
	uranusCam->Initialize(camOrbit, uranus->position - camOrbit, uranus->position);
	OrbitCameraObject* uranusCamObject = new OrbitCameraObject{ uranusCam, uranus };
	cameras.push_back(uranusCamObject);
	game->components.push_back(uranusCam);

	//FPS camera
	mainFPS = new FPSCamera(game);
	mainFPS->Initialize();
	mainFPS->SetLookPoint(Vector3(3.0f, -3.0f, 3.0f));
	mainFPS->SetTarget(Vector3(0.0f, 0.0f, 0.0f));
	game->activeCamera = mainFPS;
	game->components.push_back(mainFPS);

	//Array of lines

	for (int i = 0; i < 100; i++) {
		lines.push_back(Vector4(-100.0f, 0.0f, -50.0f + i, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		lines.push_back(Vector4(100.0f, 0.0f, -50.0f + i, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	for (int i = 0; i < 100; i++) {
		lines.push_back(Vector4(-50.0f + i, 0.0f, -100.0f, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
		lines.push_back(Vector4(-50.0f + i, 0.0f, 100.0f, 1.0f));
		lines.push_back(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	LinelistComponent* linesTriangle1 = new LinelistComponent(game);
	linesTriangle1->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	game->components.push_back(linesTriangle1);

	LinelistComponent* linesTriangle2 = new LinelistComponent(game);
	linesTriangle2->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	linesTriangle2->transforms.rotate = Matrix::CreateRotationY(DirectX::XM_PIDIV2);
	game->components.push_back(linesTriangle2);
}

void PlanetSystem::ChangeCamera(int cameraID)
{
	if (cameraID == -1) Game::getInstance()->activeCamera = mainFPS;
	else Game::getInstance()->activeCamera = cameras[cameraID]->camera;
}


void PlanetSystem::Update()
{
}

void PlanetSystem::UpdateInterval(float deltaTime)
{
	for (PlanetObject* object : objects) {
		object->mesh->transforms.move = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
		object->mesh->Update();

		object->spinAngle += object->spinSpeed * deltaTime;
		object->mesh->transforms.rotate = Matrix::CreateRotationY(object->spinAngle);
		object->mesh->Update();

		object->orbitalAngle += object->orbitalSpeed * deltaTime;
		Vector3 newObjectPosition = Vector3::Transform(object->orbit, Matrix::CreateRotationY(object->orbitalAngle));
		newObjectPosition += object->parent->position;
		object->position = newObjectPosition;
		object->mesh->transforms.move = Matrix::CreateTranslation(newObjectPosition);
	}

	for (OrbitCameraObject* cam : cameras) {
		cam->camera->SetTarget(cam->cameraOwner->position);
		//cam->camera->SetLookPoint(cam->cameraOwner->position + cam->camera->orbit);
		cam->camera->SetLookPoint(cam->cameraOwner->position + Vector3::Transform(cam->camera->orbit, Matrix::CreateRotationY(cam->cameraOwner->orbitalAngle)));
	}

	for (int i = 0; i <= cameras.size(); i++) {
		Keys key = static_cast<Keys>(static_cast<int>(Keys::D0) + i);
		if (game->inputDevice->IsKeyDown(key)) {
			ChangeCamera(i-1);
		}
	}
}

void PlanetSystem::DestroyResources()
{
}
