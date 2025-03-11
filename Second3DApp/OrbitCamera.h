#pragma once

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "Camera.h"

#include <unordered_set>
#include "Keys.h"

class OrbitCamera : public Camera
{
private:
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	float cameraSensitivity = 0.005f;
	float cameraSpeed = 0.5f;

	float cameraYaw;
	float cameraPitch;
public:
	Vector3 orbit;

	OrbitCamera(Game* gameInput) : Camera(gameInput) {};

	void Initialize(Vector3 orbitInput, Vector3 lookPointInput, Vector3 targetInput);

	void SetLookPoint(Vector3 lookPointInput);
	void SetTarget(Vector3 targetInput);
	void SetUpAxis(Vector3 upAxisInput);

	void CameraRotate(Vector2 mouseInput);
	void CameraMove(float deltaTime);

	void Update();

	void DestroyResources();

};

