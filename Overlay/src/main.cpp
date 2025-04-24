#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <iostream>
#include <string>

#include <memory/memify.h>
#include "offsets.h"
#include "structs.h"
#include "mathUtils.h"

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

void drawText(ImDrawList* drawer, float x, float y, const char* text, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	float textWidth = ImGui::CalcTextSize(text).x;
	drawer->AddText({ x - textWidth * 0.5f, y }, color, text);
}

void drawText(ImDrawList* drawer, float x, float y, int value, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	char text[50];
	sprintf_s(text, "%d", value);
	float textWidth = ImGui::CalcTextSize(text).x;
	drawer->AddText({ x - textWidth * 0.5f, y }, color, text);
}

void drawCircle(ImDrawList* drawer, float x, float y, float radius, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	drawer->AddCircleFilled({ x, y }, radius, color);
}

void drawBox(ImDrawList* drawer, float xMin, float yMin, float xMax, float yMax, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	drawer->AddRect(ImVec2(xMin, yMin), ImVec2(xMax, yMax), color, 2.f);
}


// TODO: Move to an overloading function of * in Vector3 and maybe mat4




Vector4 multiplyMat4Vec4(Vector4 vec, Matrix4 mat) {
	Vector4 result;
	result.x = vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2] + vec.w * mat[3];
	result.y = vec.x * mat[4] + vec.y * mat[5] + vec.z * mat[6] + vec.w * mat[7];
	result.z = vec.x * mat[8] + vec.y * mat[9] + vec.z * mat[10] + vec.w * mat[11];
	result.w = vec.x * mat[12] + vec.y * mat[13] + vec.z * mat[14] + vec.w * mat[15];
	return result;
}

struct Vector2 {
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}

	
};

Vector2 worldToScreen(Vector3 worldPos, Matrix4 viewMatrix, Vector2 screenDimensions) {
	Vector4 transformed = multiplyMat4Vec4(Vector4(worldPos, 1.f), viewMatrix);
	Vector2 screenPos;

	if (transformed.w <= 0.0f) {
		return Vector2(-1, -1); // off-screen or invalid
	}

	// Perspective divide to get NDC
	Vector3 ndc;
	ndc.x = transformed.x / transformed.w;
	ndc.y = transformed.y / transformed.w;
	ndc.z = transformed.z / transformed.w;

	// Perspective divide
	if (transformed.w != 0) {
		screenPos.x = (ndc.x * 0.5f + 0.5f) * screenDimensions.x;
		screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * screenDimensions.y; // Flip Y
	}

	return screenPos;
}

Vector2 screenDim(0, 0);

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
	screenDim.x = GetSystemMetrics(SM_CXSCREEN);
	screenDim.y = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"Overlay Class";

	RegisterClassExW(&wc);

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"Overlay",
		WS_POPUP,
		0, 0,
		screenDim.x, screenDim.y,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);



	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
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

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* back_buffer{ nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	if (back_buffer) {
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else {
		return 1;
	}

	ShowWindow(window, cmd_show);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, device_context);

	memify mem("cs2.exe");

	uintptr_t client = mem.GetBase("client.dll");

	std::string console = "";

	bool running = true;

	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}
		}

		if (!running) {
			break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		ImDrawList* s = ImGui::GetBackgroundDrawList();
		
		//bool open = mem.ProcessIsOpen("cs2.exe");
		
		//client = mem.GetBase("client.dll");
		
		Matrix4 viewMatrix = mem.Read<Matrix4>(client + dwViewMatrix);
		uintptr_t entityList = mem.Read<uintptr_t>(client + dwEntityList);
		uintptr_t listEntry = mem.Read<uintptr_t>(entityList + 0x10);

		for (int i = 0; i < 64; i++) {
			uintptr_t currentController = mem.Read<uintptr_t>(listEntry + i * 0x78);

			if (currentController == 0) {
				continue;
			}
			
			int pawnhandle = mem.Read<int>(currentController + m_hPlayerPawn);

			if (pawnhandle == 0) {
				continue;
			}

			uintptr_t listEntry2 = mem.Read<uintptr_t>(entityList + 0x8 * ((pawnhandle & 0x7FFF) >> 9) + 0x10);

			uintptr_t currentPawn = mem.Read<uintptr_t>(listEntry2 + 0x78 * (pawnhandle & 0x1FF));
			
			if (currentPawn == 0) {
				continue;
			}

			// you can get the gamescenenode earlier without getting the playerpawn because the controller has the pointer to gamescenenode too
			uintptr_t gameSceneNode = mem.Read<uintptr_t>(currentPawn + m_pGameSceneNode);
			int32_t health = mem.Read<uintptr_t>(currentPawn + m_iHealth);
			CGameSceneNode gameSceneNodeStruct = mem.Read<CGameSceneNode>(gameSceneNode);
			
			{
				// Make a box around the player
				Vector3 bottomPos = mem.Read<Vector3>(gameSceneNode + m_vecAbsOrigin);
				Vector2 bottomScreenPos = worldToScreen(bottomPos, viewMatrix, screenDim);
				Vector3 topPos = bottomPos + Vector3(0.f, 0.f, 72.f);
				Vector2 topScreenPos = worldToScreen(topPos, viewMatrix, screenDim);
				drawBox(s, topScreenPos.x - 30.f, topScreenPos.y, bottomScreenPos.x + 30.f, bottomScreenPos.y);
				drawText(s, topScreenPos.x, topScreenPos.y - 12.f, health);
			}
		}
		
		drawText(s, 1100.f, 20.f, console.c_str());
		
		console += std::string("\n\n");
		
		if (console.length() > 500) {
			console.erase(0, 400);
		}
		// rendering goes here

		ImGui::Render();

		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
		device_context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(1U, 0U); // set 1st arg to 0 to turn off vsync
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();

	if (swap_chain) {
		swap_chain->Release();
	}

	if (device_context) {
		device_context->Release();
	}

	if (device) {
		device->Release();
	}

	if (render_target_view) {
		render_target_view->Release();
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}