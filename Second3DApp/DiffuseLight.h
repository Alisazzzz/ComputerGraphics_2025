#pragma once
#include "GameComponent.h";

class DiffuseLight : public GameComponent
{
public:
	DiffuseLight(Game* gameInput) : GameComponent(gameInput) {};

	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;

	Vector4 position;
	Quaternion rotation;

	float attenuation_a = 1.0f;
	float attenuation_b = 0.5f;
	float attenuation_c = 0.1f;
	Vector4 lightAttenuation;

	void Initialize();
};

