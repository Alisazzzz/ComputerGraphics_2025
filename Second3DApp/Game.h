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

#include "TexturedTriangle.h"
#include "InputDevice.h"
#include "Camera.h"
#include "FPSCamera.h"
#include "OrbitCamera.h"
#include "Lighting.h"
#include "ShadowMap.h"

class DisplayWin32;

class Game
{
private:
	static Game* gameInstance;
	Game() {};

	int SHADOW_MAP_SIZE = 2048;

	bool started = false;

public:

	bool isPong = false;
	bool isPlanetSystem = false;
	bool isKatamari = false;

	DisplayWin32* window;
	std::vector<GameComponent*> components;
	std::vector<TexturedTriangle*> meshes;
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

	//shadows
	Matrix lightView;
	Matrix lightProjection;
	ShadowMapClass* dirLightShadows;

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
	FPSCamera* mainFPS;

	std::vector<PointLight*> pntLights;
	DirectionalLight* dirLight;

	void Initialize(int screenWidthInput, int screenHeightInput);
	void CreateBackBuffer();
	void CreateDepthBuffer();

	void Draw();
	void Update();
	void EndFrame();
	int Exit();
	void UpdateLight();
	void RenderShadowMap();

	void PrepareFrame();

	void UpdateInterval();
	void MessageHandler();

	void MouseInputHandler(Vector2 mouseInput);

	void Run();
	void Resize();

	void PongGame();
	void PlanetSystemView();
	void KatamariGame();
};