#include "Game.h"

#include "DisplayWin32.h"
#include "TriangleComponent.h"
#include "MeshGenerator.h"

#include "Pong.h"

Game* Game::gameInstance = nullptr;

void Game::Initialize(int screenWidthInput, int screenHeightInput)
{
	screenWidth = screenWidthInput;
	screenHeight = screenHeightInput;

	window = new DisplayWin32(screenWidth, screenHeight, applicationName);
	window->Display();

	inputDevice = new InputDevice(getInstance());

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = screenWidth;
	swapDesc.BufferDesc.Height = screenHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = window->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	CreateBackBuffer();

	res = device->CreateRenderTargetView(backBuffer, nullptr, &renderView);

	std::vector<UINT> strides = { 32 }; 
	std::vector<UINT> offsets = { 0 };

	/*
	TriangleComponent* square = new TriangleComponent(getInstance());
	Mesh squareMesh = MeshGenerator::getInstance()->getSquare(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	square->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", squareMesh.points, squareMesh.indeces, strides, offsets);
	components.push_back(square);

	TriangleComponent* star = new TriangleComponent(getInstance());
	Mesh starMesh = MeshGenerator::getInstance()->getStar(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	star->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", starMesh.points, starMesh.indeces, strides, offsets);
	components.push_back(star);
	*/

	TriangleComponent* cube = new TriangleComponent(getInstance());
	Mesh squareMesh = MeshGenerator::getInstance()->getCube(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	cube->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", squareMesh.points, squareMesh.indeces, strides, offsets);
	components.push_back(cube);

	camm = new OrbitCamera(getInstance());
	//camm = new FPSCamera(getInstance());
	camm->Initialize();
	activeCamera = camm;
	components.push_back(camm);
}

void Game::CreateBackBuffer()
{
	auto res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
}

void Game::Draw()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(renderView, color);

	context->OMSetRenderTargets(1, &renderView, nullptr);

	for (GameComponent* component : components) {
		component->Draw();
	}
	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::Update()
{
	if (isPong) {
		Pong* pong = Pong::getInstance();
		pong->Update();
	};

	//Matrix view = Matrix::CreateLookAt(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	//view = view.Transpose();
	//Matrix proj = Matrix::CreatePerspectiveFieldOfView(180.0f, aspectRatio, 0.01f, 100.0f);
	//Matrix proj = Matrix::CreateOrthographic(orthoWidth, orthoHeight, 0.01f, 100.0f);

	for (GameComponent* component : components) {
		component->Update();
	}
}

void Game::EndFrame()
{
}

int Game::Exit()
{
	delete window;
	delete inputDevice;

	for (GameComponent* component : components) {
		component->DestroyResources();
		delete component;
	}

	if (isPong) {
		Pong::getInstance()->DestroyResources();
		delete Pong::getInstance();
	}

	context->Release();
	device->Release();

	swapChain->Release();

	backBuffer->Release();
	renderView->Release();

	std::cout << "Hello World!\n";
	return 0;
}

void Game::PrepareFrame() 
{
	context->ClearState();

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
}

void Game::UpdateInterval()
{
	auto curTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;
		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(window->hWnd, text);

		frameCount = 0;
	}

	if (isPong) {
		Pong::getInstance()->UpdateInterval(deltaTime);
	}

	PrepareFrame();
	Update();
	Draw();
	EndFrame();
}

void Game::MessageHandler()
{
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		isExitRequested = true;
	}
}

void Game::Run()
{
	PrevTime = std::chrono::steady_clock::now();
	totalTime = 0;
	frameCount = 0;

	while (!isExitRequested) {
		MessageHandler();
		UpdateInterval();
	}
	Exit();
}

void Game::Resize()
{
}

void Game::CameraUpdate(Vector2 mouseInput, std::unordered_set<Keys>* keys)
{
	activeCamera->CameraRotate(mouseInput);

	auto curTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;

	activeCamera->CameraMove(keys, deltaTime);
}

void Game::PongGame()
{
	isPong = true;
	Pong* pongGame = Pong::getInstance();

	pongGame->Initialize();
}
