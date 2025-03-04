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

class OrthoCamera : public Camera
{
private:
	float aspectRatio;

	float orthoWidth;
	float orthoHeight;
	float nearPlane;
	float farPlane;

public:

	OrthoCamera(Game* gameInput) : Camera(gameInput) {};

	void Initialize();

	void SetLookPoint(Vector3 lookPointInput);
	void SetTarget(Vector3 targetInput);
	void SetUpAxis(Vector3 upAxisInput);

	void SetOrthoHeight(float heightInput);

	void Update();

	void DestroyResources();
};

