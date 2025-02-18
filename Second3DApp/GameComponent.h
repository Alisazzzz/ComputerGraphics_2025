#pragma once

#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game;

class GameComponent
{
public:
	Game* game;

	GameComponent(Game* gameInput) : game(gameInput) {}

	void Initialize() {};

	virtual void Draw() {};
	virtual void Update() {};

	void Reload() {};
	
	virtual void DestroyResources() {};

};

