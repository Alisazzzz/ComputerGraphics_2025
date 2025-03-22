#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

#include "MeshGenerator.h"
#include "TriangleComponent.h"
#include "TexturedTriangle.h"

#include "OrbitCamera.h"

class Game;
class Katamari;

struct Pickable {
	std::vector<TexturedTriangle*> mesh;
	//DirectX::BoundingBox collision;
	DirectX::BoundingOrientedBox collision;
	Vector3 position;
	Vector3 rotation;
	Matrix tranformations;
};

using namespace DirectX::SimpleMath;

class KatamariBall 
{
private:
	Game* game;
	Katamari* katamariGame;

	//TriangleComponent* katamariMesh; 
	TexturedTriangle* katamariMesh;
	DirectX::BoundingSphere collision;

	OrbitCamera* mainOrbit;

	Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
	float collisionScale = 1.0f;

	float speed = 0.01f;
	float rotationSpeed = 7.0f;
	float radius = 0.5f;

	std::vector<Pickable*> collected;

	void MoveKatamari(float deltaTime);

public:
	KatamariBall(Game* gameInput);

	void CollisionCheck();

	void UpdateInterval(float deltaTime);
	void Update();

	OrbitCamera* getCamera();
};

