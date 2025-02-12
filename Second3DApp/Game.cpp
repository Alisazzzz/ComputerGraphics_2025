#include "Game.h"

void Game::Initialize()
{
	gameComp = GameComponent();
	window = DisplayWin32(screenHeight, screenWidth, applicationName);
	window.Display();

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
	swapDesc.OutputWindow = window.hWnd;
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

	gameComp.Initialize(device);
}

void Game::CreateBackBuffer()
{
	auto res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);	// __uuidof(ID3D11Texture2D)
}

void Game::Draw()
{
	float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	context->DrawIndexed(6, 0, 0);

	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::EndFrame()
{
}

int Game::Exit()
{
	std::cout << "Hello World!\n";
	return 0;
}

void Game::PrepareFrame()
{
	context->ClearState();

	gameComp.Draw(context);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &renderView, nullptr);
}

void Game::Update()
{
	PrepareFrame();

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
		SetWindowText(window.hWnd, text);

		frameCount = 0;
	}

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
		Update();
	}
	Exit();
}
