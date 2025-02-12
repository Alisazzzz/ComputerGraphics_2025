#include "GameComponent.h"

void GameComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	triangles.Initialize(device);
}

void GameComponent::Draw(ID3D11DeviceContext* context)
{
	triangles.Draw(context);
}

void GameComponent::DestroyResources()
{
}

void GameComponent::Reload()
{
}

void GameComponent::Update()
{
}
