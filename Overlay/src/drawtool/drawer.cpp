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
	// Init Windows windowclass
	WNDCLASSEXW wc = makeWindowClass(L"Overlay Class", instance);
	RegisterClassExW(&wc);

	
}

Drawer::~Drawer()
{
}

WNDCLASSEXW makeWindowClass(const wchar_t* className, HINSTANCE applicationInstance) {
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = applicationInstance;
	wc.lpszClassName = className;
	return wc;
}

const HWND makeWindow(WNDCLASSEXW windowClass, HINSTANCE instance, float width, float height, const wchar_t* windowName) {
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

void adjustWindowRect(HWND window) {
	
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

DXGI_SWAP_CHAIN_DESC makeSwapChainDesc(HWND window, UINT targetfps) {
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