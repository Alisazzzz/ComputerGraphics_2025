#include "FPSCamera.h"
#include "Game.h"

#include <unordered_set>
#include "Keys.h"

void FPSCamera::Initialize()
{
	lookPoint = Vector3(0.0f, 0.0f, -1.0f);
	target = Vector3(0.0f, 0.0f, 1.0f);
	upAxis = Vector3(0.0f, 1.0f, 0.0f);

	fov = 30.0f;
	aspectRatio = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);
	nearPlane = 0.01f;
	farPlane = 100.0f;

	cameraInfo.projection = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);

	velocity = Vector3(0.0f, 0.0f, 0.0f);

	cameraYaw = atan2(target.z, target.x);
	cameraPitch = asin(target.y);
}

void FPSCamera::SetLookPoint(Vector3 lookPointInput)
{
	lookPoint = lookPointInput;
}

void FPSCamera::SetTarget(Vector3 targetInput)
{
	target = targetInput;
}

void FPSCamera::SetUpAxis(Vector3 upAxisInput)
{
	upAxis = upAxisInput;
}

void FPSCamera::CameraRotate(Vector2 mouseInput)
{
	cameraYaw += mouseInput.x * cameraSensitivity;
	cameraPitch += mouseInput.y * cameraSensitivity;

	float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;
	if (cameraPitch < -pitchLimit) cameraPitch = -pitchLimit;
	if (cameraPitch > pitchLimit) cameraPitch = pitchLimit;

	Vector3 direction;
	direction.x = - cosf(cameraYaw) * cosf(cameraPitch);
	direction.y = sinf(cameraPitch);
	direction.z = cosf(cameraPitch) * sinf(cameraYaw);
	direction.Normalize();

	target = lookPoint + direction;
}

void FPSCamera::CameraMove(std::unordered_set<Keys>* keys, float deltaTime)
{
	Vector3 forward = target - lookPoint;
	forward.Normalize();

	Vector3 right = upAxis.Cross(forward);
	right.Normalize();

	float speed = 4.0f * deltaTime;

	if (keys->count(Keys::W)) velocity = forward * speed;
	if (keys->count(Keys::S)) velocity = -forward * speed;
	if (keys->count(Keys::A)) velocity = -right * speed;
	if (keys->count(Keys::D)) velocity = right * speed;

	if (!keys->count(Keys::W) && !keys->count(Keys::S) && !keys->count(Keys::A) && !keys->count(Keys::D)) 
		velocity = Vector3(0.0f, 0.0f, 0.0f);
}


void FPSCamera::Update()
{
	lookPoint += velocity * cameraSpeed;
	target += velocity * cameraSpeed;
	cameraInfo.projection = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	cameraInfo.view = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void FPSCamera::DestroyResources() {}
