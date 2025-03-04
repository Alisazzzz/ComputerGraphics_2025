#pragma once

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include <unordered_set>
#include "Keys.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "GameComponent.h"

struct CameraMatrix {
	Matrix view;
	Matrix projection;
};

class Camera : public GameComponent
{
public:
	CameraMatrix cameraInfo;

	Vector3 lookPoint;
	Vector3 target;
	Vector3 upAxis;

	Camera(Game* gameInput) : GameComponent(gameInput) {}

	void Initialize() {};

	virtual void CameraRotate(Vector2 mouseInput) {};
	virtual void CameraMove(std::unordered_set<Keys>* keys, float deltaTime) {};

	virtual void Draw() {};
	virtual void Update() {};

	virtual void DestroyResources() {};
};

