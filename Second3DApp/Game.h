#pragma once

#include <wrl.h>
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

#include "GameComponent.h"
#include "DisplayWin32.h"

class Game
{
private:
	GameComponent gameComp;
	DisplayWin32 window;

	MSG msg = {};
	bool isExitRequested = false;

	int screenWidth;
	int screenHeight;

	LPCWSTR applicationName = L"My3DApp";
	
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* renderView;
	ID3D11DeviceContext* context;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	IDXGISwapChain* swapChain;

	float totalTime = 0;
	unsigned int frameCount = 0;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;

public:
	Game(int screenWidthInput, int screenHeightInput) 
	{
		screenWidth = screenWidthInput;
		screenHeight = screenHeightInput;
	};

	void Initialize();
	void CreateBackBuffer();

	void Draw();
	void EndFrame();
	int Exit();

	void PrepareFrame();

	void Update();
	void MessageHandler();
	void Run();
};

