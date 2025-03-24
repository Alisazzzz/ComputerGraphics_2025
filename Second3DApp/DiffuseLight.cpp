#include "DiffuseLight.h"

void DiffuseLight::Initialize()
{
	position = Vector4(2.0f, -2.0f, 2.0f, 1.0f);
	rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f);
	lightAttenuation = Vector4(attenuation_a, attenuation_b, attenuation_c, 1.0f);
}
