#include "drawer.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
		return 0L;
	}

	if (message == WM_DESTROY) {
		PostQuitMessage(0);
		return 0L;
	}

	return DefWindowProc(window, message, w_param, l_param);
}

Drawer::Drawer(float width, float height, UINT fps, HINSTANCE instance, INT cmdShow)
{

	// -- Init Windows/D3D11 --
	// Init Windows windowclass
	Drawer::windowClass = makeWindowClass(L"Overlay Class", instance);
	RegisterClassExW(&windowClass);

	Drawer::window = makeWindow(windowClass, instance, width, height, L"Overlay");
	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
	adjustWindowRect(window);

	Drawer::swapChainDesc = makeSwapChainDesc(window, 60U);

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		&level,
		&deviceContext
	);

	swapChain->GetBuffer(0U, IID_PPV_ARGS(&backBuffer));

	if (backBuffer) {
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}
	else {
		throw std::runtime_error("Failed to create back buffer");
	}

	ShowWindow(window, cmdShow);
	UpdateWindow(window);

	// -- Init ImGui --
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, deviceContext);
}

Drawer::~Drawer()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();

	if (swapChain) {
		swapChain->Release();
	}

	if (deviceContext) {
		deviceContext->Release();
	}

	if (device) {
		device->Release();
	}

	if (renderTargetView) {
		renderTargetView->Release();
	}

	DestroyWindow(window);
	UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
}


void Drawer::initFrame(void (*quitFunction)()) {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			quitFunction();
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	Drawer::backgroundDrawList = ImGui::GetBackgroundDrawList();
}

void Drawer::drawFrame() {
	ImGui::Render();

	const float color[4]{ 0.f, 0.f, 0.f, 0.f };
	deviceContext->OMSetRenderTargets(1U, &renderTargetView, nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1U, 0U); // set 1st arg to 0 to turn off vsync
}

void Drawer::drawBox(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color, float rounding) {
	backgroundDrawList->AddRect(
		ImVec2(xMinimum, yMinimum),
		ImVec2(xMaximum, yMaximum),
		color,
		rounding
	);
}

void Drawer::drawBoxFilled(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color, float rounding) {
	backgroundDrawList->AddRectFilled(
		ImVec2(xMinimum, yMinimum),
		ImVec2(xMaximum, yMaximum),
		color,
		rounding
	);
}

void Drawer::drawLine(float xStart, float yStart, float xEnd, float yEnd, ImColor color, float thickness) {
	backgroundDrawList->AddLine(
		ImVec2(xStart, yStart),
		ImVec2(xEnd, yEnd),
		color,
		thickness
	);
}

void Drawer::drawCircle(float xCenter, float yCenter, float radius, ImColor color) {
	backgroundDrawList->AddCircle(
		ImVec2(xCenter, yCenter),
		radius,
		color
	);
}

void Drawer::drawCircleFilled(float xCenter, float yCenter, float radius, ImColor color) {
	backgroundDrawList->AddCircleFilled(
		ImVec2(xCenter, yCenter),
		radius,
		color
	);
}

void Drawer::drawText(char* text, float x, float y, ImColor color) {
	backgroundDrawList->AddText(
		ImVec2(x, y),
		color,
		text
	);
}

void Drawer::drawTextCentered(char* text, float x, float y, ImColor color) {
	ImVec2 textSize = ImGui::CalcTextSize(text);
	backgroundDrawList->AddText(
		ImVec2(x - textSize.x / 2.f, y - textSize.y / 2.f),
		color,
		text
	);
}

WNDCLASSEXW Drawer::makeWindowClass(const wchar_t* className, HINSTANCE applicationInstance) {
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = applicationInstance;
	wc.lpszClassName = className;
	return wc;
}

const HWND Drawer::makeWindow(WNDCLASSEXW windowClass, HINSTANCE instance, float width, float height, const wchar_t* windowName) {
	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		windowClass.lpszClassName,
		windowName,
		WS_POPUP,
		0, 0,
		width, height,
		nullptr,
		nullptr,
		windowClass.hInstance,
		nullptr
	);
	return window;
}

void Drawer::adjustWindowRect(HWND window) {
	
	RECT client_area{};
	GetClientRect(window, &client_area);

	RECT window_area{};
	GetWindowRect(window, &window_area);

	POINT diff{};
	ClientToScreen(window, &diff);

	const MARGINS margins{
		window_area.left + (diff.x - window_area.left),
		window_area.top + (diff.y - window_area.top),
		client_area.right,
		client_area.bottom
	};

	DwmExtendFrameIntoClientArea(window, &margins);
	
}

DXGI_SWAP_CHAIN_DESC Drawer::makeSwapChainDesc(HWND window, UINT targetfps) {
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = targetfps;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return sd;
}