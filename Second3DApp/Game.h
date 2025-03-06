#pragma once

#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <vector>
#include <mutex>

#include <SimpleMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "GameComponent.h"
#include "InputDevice.h"
#include "Camera.h"
#include "FPSCamera.h"
#include "OrbitCamera.h"

#include <unordered_set>
#include "Keys.h"

class DisplayWin32;

class Game
{
private:
	static Game* gameInstance;
	Game() {};

public:

	bool isPong = false;
	bool isPlanetSystem = false;

	DisplayWin32* window;
	std::vector<GameComponent*> components;
	InputDevice* inputDevice;

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

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	float totalTime = 0;
	unsigned int frameCount = 0;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;

	Game(const Game& gameObject) = delete;

	static Game* getInstance() {
		if (gameInstance == nullptr) {
			gameInstance = new Game();
		}
		return gameInstance;
	}

	Camera* activeCamera;
	//OrbitCamera* camm;
	//FPSCamera* camm;

	void Initialize(int screenWidthInput, int screenHeightInput);
	void CreateBackBuffer();

	void Draw();
	void Update();
	void EndFrame();
	int Exit();

	void PrepareFrame();

	void UpdateInterval();
	void MessageHandler();

	void KeyInputHadnler(std::unordered_set<Keys>* keys);
	void MouseInputHandler(Vector2 mouseInput);

	void Run();
	void Resize();

	void PongGame();
	void PlanetSystemView();
};