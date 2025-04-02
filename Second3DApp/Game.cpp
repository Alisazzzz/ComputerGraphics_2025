#include "Game.h"

#include "DisplayWin32.h"
#include "TriangleComponent.h"
#include "TexturedTriangle.h"
#include "MeshGenerator.h"

#include "Pong.h"
#include "PlanetSystem.h"
#include "Katamari.h"

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
	
	depthStencilBuffer = nullptr;
	depthStencilView = nullptr;

	CreateDepthBuffer();

	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

	std::vector<DirectX::XMFLOAT4> lines;

	for (int i = 0; i <= 400; i++) {
		lines.push_back(Vector4(-100.0f, 1.0f, -100.0f + i, 1.0f));
		lines.push_back(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
		lines.push_back(Vector4(100.0f, 1.0f, -100.0f + i, 1.0f));
		lines.push_back(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	}

	LinelistComponent* linesTriangle1 = new LinelistComponent(getInstance());
	linesTriangle1->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	components.push_back(linesTriangle1);

	LinelistComponent* linesTriangle2 = new LinelistComponent(getInstance());
	linesTriangle2->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", lines, strides, offsets, true);
	linesTriangle2->transforms.rotate = Matrix::CreateRotationY(DirectX::XM_PIDIV2);
	components.push_back(linesTriangle2);

	//lights and shadows
	pntLights = {};
	dirLight = nullptr;

	shadowMapTexture = nullptr;
	shadowMapDSV = nullptr;
	shadowMapSRV = nullptr;
	shadowSampler = nullptr;
}

void Game::CreateBackBuffer()
{
	auto res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
}

void Game::CreateDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	context->OMSetRenderTargets(1, &renderView, depthStencilView);

	ID3D11DepthStencilState* depthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = FALSE;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	
	context->OMSetDepthStencilState(depthStencilState, 1);
}

void Game::CreateShadowMapResources()
{
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(shadowMapDesc));
	shadowMapDesc.Width = SHADOW_MAP_SIZE;
	shadowMapDesc.Height = SHADOW_MAP_SIZE;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.SampleDesc.Quality = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;
	device->CreateTexture2D(&shadowMapDesc, nullptr, &shadowMapTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowMapTexture, &dsvDesc, &shadowMapDSV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowMapTexture, &srvDesc, &shadowMapSRV);

	D3D11_SAMPLER_DESC shadowSamplerDesc;
	ZeroMemory(&shadowSamplerDesc, sizeof(shadowSamplerDesc));
	shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.BorderColor[0] = 1.0f;
	shadowSamplerDesc.BorderColor[1] = 1.0f;
	shadowSamplerDesc.BorderColor[2] = 1.0f;
	shadowSamplerDesc.BorderColor[3] = 1.0f;
	shadowSamplerDesc.MinLOD = 0;
	shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSamplerDesc.MaxAnisotropy = 1;
	device->CreateSamplerState(&shadowSamplerDesc, &shadowSampler);
}

void Game::Draw()
{
	//float color[] = { 0.59f, 0.77f, 0.78f, 1.0f };
	//float color[] = { 0.84f, 0.47f, 0.65f, 1.0f };
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	context->ClearRenderTargetView(renderView, color);

	context->OMSetRenderTargets(1, &renderView, depthStencilView);

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

	if (isKatamari) {
		Katamari* katamari = Katamari::getInstance();
		katamari->Update();
	}

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

	if (pntLights.size() != 0) {
		for (int i = 0; i < pntLights.size(); i++)
			delete pntLights[i];
	}
	if (dirLight != nullptr) delete dirLight;

	std::cout << "Hello World!\n";
	return 0;
}

void Game::UpdateLight()
{
	Vector3 sceneCenter(0.0f, 0.0f, 0.0f);
	Vector3 lightCameraPos = sceneCenter - dirLight->direction * 50.0f;

	Matrix lightView = Matrix::CreateLookAt(lightCameraPos, sceneCenter, Vector3::Up);

	float shadowArea = 50.0f;
	Matrix lightProjection = Matrix::CreateOrthographic(shadowArea, shadowArea, 0.1f, 100.0f);

	lightViewProjection = lightView * lightProjection;
}

void Game::RenderShadowMap()
{
	D3D11_VIEWPORT oldViewport;
	UINT numViewports = 1;
	context->RSGetViewports(&numViewports, &oldViewport);

	D3D11_VIEWPORT shadowViewport;
	shadowViewport.Width = static_cast<float>(SHADOW_MAP_SIZE);
	shadowViewport.Height = static_cast<float>(SHADOW_MAP_SIZE);
	shadowViewport.MinDepth = 0.0f;
	shadowViewport.MaxDepth = 1.0f;
	shadowViewport.TopLeftX = 0.0f;
	shadowViewport.TopLeftY = 0.0f;
	context->RSSetViewports(1, &shadowViewport);

	context->ClearDepthStencilView(shadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->OMSetRenderTargets(0, nullptr, shadowMapDSV);

	for (TexturedTriangle* mesh : meshes)
	{
		// Устанавливаем шейдер для shadow map (см. ниже)
		// Устанавливаем матрицу преобразования (world * lightViewProjection)
		//Matrix worldViewProj = mesh->transforms * lightViewProjection;
		// Передаем матрицу в шейдер
		// Рендерим mesh
		//mesh.Render(context);
	}

	context->RSSetViewports(1, &oldViewport);
}

void Game::PrepareFrame() 
{
	context->ClearState();
	if(depthStencilView != nullptr)
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

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

	activeCamera->CameraMove(deltaTime);

	if (isPong) {
		Pong::getInstance()->UpdateInterval(deltaTime);
	}

	if (isPlanetSystem) {
		PlanetSystem::getInstance()->UpdateInterval(deltaTime);
	}

	if (isKatamari) {
		Katamari::getInstance()->UpdateInterval(deltaTime);
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

void Game::MouseInputHandler(Vector2 mouseInput)
{
	activeCamera->CameraRotate(mouseInput);
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

void Game::PongGame()
{
	isPong = true;
	Pong* pongGame = Pong::getInstance();
	pongGame->Initialize();
}

void Game::PlanetSystemView()
{
	isPlanetSystem = true;
	PlanetSystem* planetGame = PlanetSystem::getInstance();
	CreateDepthBuffer();
	planetGame->Initialize();
}

void Game::KatamariGame()
{
	isKatamari = true;
	Katamari* katamariGame = Katamari::getInstance();
	CreateDepthBuffer();
	CreateShadowMapResources();
	katamariGame->Initialize();
}
