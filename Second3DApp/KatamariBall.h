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
	DirectX::BoundingOrientedBox collision;
	Vector3 position;
	Quaternion rotation;
	bool collected;
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
	Vector3 position = Vector3(0.0f, -1.0f, 0.0f);
	Quaternion rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix rotationDelta = Matrix::CreateFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f);
	
	float collisionScale = 1.0f;

	float speed = 0.00f;
	float maxSpeed = 5.00f;
	float radius = 1.0f;
	bool movingForward = true;

	bool isJumping = false;
	bool isFalling = false;
	float a = 0.21f;
	float heightMax = 0.21f;
	float jumpHeight = 0.0f;

	std::vector<Pickable*> collected;
	std::vector<Vector3> lightsOrbits;

	float Inertia(float speed, bool moving);
	void Jump(float deltaTime);
	void AddingLights();
	void MoveKatamari(float deltaTime);

public:
	KatamariBall(Game* gameInput);

	void CollisionCheck();

	void UpdateInterval(float deltaTime);
	void Update();

	OrbitCamera* getCamera();
};

