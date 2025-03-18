#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

#include "TriangleComponent.h"
#include "MeshGenerator.h"

#include "OrthoCamera.h"

class Game;

using namespace DirectX::SimpleMath;

struct PongObject {
	TriangleComponent* mesh;
	DirectX::BoundingBox collision;
	Vector3 position;
};

class Pong
{
private:
	static Pong* pongInstance;
	Pong() {};

	void updateCollision(std::vector<DirectX::XMFLOAT4> points, DirectX::BoundingBox* collision, 
		bool setCenter = false, Vector3 scale = {1.0f, 1.0f, 1.0f});
	DirectX::BoundingBox createCollision(std::vector<DirectX::XMFLOAT4> points);
	
	void StartRound(UINT playerWhoLoose);
	Vector4 ballVelocity;
	float ballSpeed = 1.0f;
	float ballSpeedGrow = 0.01f;

	DirectX::BoundingBox topCollision, bottomCollision;
	DirectX::BoundingBox leftPlayerCollision, rightPlayerCollision;

	UINT leftPlayerScore, rightPlayerScore = 0;

	UINT rightCounts = 1;
	UINT leftCounts = 1;

	std::vector<TriangleComponent*> additionals;

	OrthoCamera* camera;

public:
	PongObject* leftPaddle;
	PongObject* rightPaddle;
	PongObject* ball;

	UINT netCount = 0;
	float netUpdateTime = 0.0f;

	bool keys[256] = {};

	void Initialize();

	void UpdatePaddle(UINT paddle, UINT direction, float deltaTime);
	void UpdateBall(float deltaTime);
	void CheckCollisions();

	void Update();

	void UpdateInterval(float deltaTime);

	static Pong* getInstance() {
		if (pongInstance == nullptr) {
			pongInstance = new Pong();
		}
		return pongInstance;
	}

	void DestroyResources();
};

