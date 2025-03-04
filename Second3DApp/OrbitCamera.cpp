#include "OrbitCamera.h"
#include "Game.h"

#include <unordered_set>
#include "Keys.h"

void OrbitCamera::Initialize()
{
	lookPoint = Vector3(0.0f, 0.0f, -1.0f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	upAxis = Vector3(0.0f, 1.0f, 0.0f);

	fov = 30.0f;
	aspectRatio = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);
	nearPlane = 0.01f;
	farPlane = 100.0f;

	cameraInfo.projection = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);

	velocity = Vector3(0.0f, 0.0f, 0.0f);

	//cameraYaw = atan2(target.z, target.x);
	//cameraPitch = asin(target.y);
	cameraYaw = 0.0f;
	cameraPitch = 0.0f;
}

void OrbitCamera::SetLookPoint(Vector3 lookPointInput)
{
	lookPoint = lookPointInput;
}

void OrbitCamera::SetTarget(Vector3 targetInput)
{
	target = targetInput;
}

void OrbitCamera::SetUpAxis(Vector3 upAxisInput)
{
	upAxis = upAxisInput;
}

void OrbitCamera::CameraRotate(Vector2 mouseInput)
{
	Vector3 camPosition = Vector3(Vector3::Distance(target, lookPoint), 0.0f, 0.0f);

	cameraYaw += mouseInput.x * cameraSensitivity;
	cameraPitch -= mouseInput.y * cameraSensitivity;

	float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;
	if (cameraPitch < -pitchLimit) cameraPitch = -pitchLimit;
	if (cameraPitch > pitchLimit) cameraPitch = pitchLimit;

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(cameraYaw, 0, cameraPitch);

	lookPoint = Vector3::Transform(camPosition, rotationMatrix);

}

void OrbitCamera::CameraMove(std::unordered_set<Keys>* keys, float deltaTime) {}

void OrbitCamera::Update()
{
	cameraInfo.projection = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void OrbitCamera::DestroyResources() {}
