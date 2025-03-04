#include "OrthoCamera.h"
#include "Game.h"

void OrthoCamera::Initialize()
{
	lookPoint = Vector3(0.0f, 0.0f, 0.0f);
	target = Vector3(0.0f, 0.0f, 1.0f);
	upAxis = Vector3(0.0f, 1.0f, 0.0f);

	aspectRatio = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);
	orthoHeight = 1.0f;
	orthoWidth = orthoHeight * aspectRatio;

	nearPlane = 0.01f;
	farPlane = 100.0f;

	cameraInfo = {};
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
	cameraInfo.projection = Matrix::CreateOrthographic(orthoWidth, orthoHeight, nearPlane, farPlane);
}

void OrthoCamera::SetOrthoHeight(float heightInput)
{
	aspectRatio = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);
	orthoHeight = heightInput;
	orthoWidth = orthoHeight * aspectRatio;

	cameraInfo.projection = Matrix::CreateOrthographic(orthoWidth, orthoHeight, nearPlane, farPlane);
}

void OrthoCamera::Update() {}
void OrthoCamera::DestroyResources() {}

void OrthoCamera::SetUpAxis(Vector3 upAxisInput) {
	upAxis = upAxisInput;
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void OrthoCamera::SetLookPoint(Vector3 lookPointInput)
{
	lookPoint = lookPointInput;
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void OrthoCamera::SetTarget(Vector3 targetInput) {
	target = targetInput;
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
}
