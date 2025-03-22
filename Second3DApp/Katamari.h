#pragma once

#include <iostream>
#include <random>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

#include "TriangleComponent.h"
#include "LinelistComponent.h"
#include "TexturedTriangle.h"
#include "MeshGenerator.h"

#include "OrbitCamera.h"
#include "FPSCamera.h"

#include "KatamariBall.h"

class Game;

using namespace DirectX::SimpleMath;

class Katamari
{
private:
	static Katamari* katamariInstance;
	Katamari() {};

	OrbitCamera* mainOrbit;
	FPSCamera* mainFPS;
	Game* game;

	int objectsCount = 10;
	Vector3 ruMapCorner = Vector3(20, 0, 20);
	Vector3 ldMapCorner = Vector3(0, 0, 0);

	KatamariBall* ball;

public:
	std::vector<Pickable*> pickables;

	Katamari(const Katamari& katamari) = delete;

	static Katamari* getInstance() {
		if (katamariInstance == nullptr)
			katamariInstance = new Katamari();
		return katamariInstance;
	}

	void Initialize();
	void RandomObjectGeneration();

	void Update();
	void UpdateInterval(float deltaTime);

};

