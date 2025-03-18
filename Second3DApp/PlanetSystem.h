#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

#include "TriangleComponent.h"
#include "LinelistComponent.h"
#include "MeshGenerator.h"

#include "FPSCamera.h"
#include "OrbitCamera.h"

class Game;

using namespace DirectX::SimpleMath;


struct PlanetObject {
	TriangleComponent* mesh;
	float spinAngle;
	float spinSpeed;

	float orbitalAngle;
	float orbitalSpeed;

	Vector3 position;
	Vector3 orbit;
	PlanetObject* parent;
};

struct OrbitCameraObject {
	OrbitCamera* camera;
	PlanetObject* cameraOwner;
};

class PlanetSystem
{
private:
	static PlanetSystem* planetSystemInstance;
	PlanetSystem() {};

	std::vector<PlanetObject*> objects;
	std::vector<OrbitCameraObject*> cameras;
	FPSCamera* mainFPS;

	Game* game;

public:

	std::vector<DirectX::XMFLOAT4> lines;

	PlanetSystem(const PlanetSystem& planetSystemObject) = delete;

	static PlanetSystem* getInstance() {
		if (planetSystemInstance == nullptr) {
			planetSystemInstance = new PlanetSystem();
		}
		return planetSystemInstance;
	}

	void Initialize();

	void ChangeCamera(int cameraID);

	void Update();
	void UpdateInterval(float deltaTime);

	void DestroyResources();
};

