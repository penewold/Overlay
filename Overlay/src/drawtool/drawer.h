#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>

#include <stdexcept>
/*
* ------ PLANNED USAGE ------
* Drawer drawer(width, height, fps, instance, cmdShow);
* 
* 
* bool running = true;
* while (running) {
*	drawer.initFrame();
*	
*   // program logic begin
*	
*	drawer.drawBox(xMin, yMin, xMax, yMax, color, rounding);
*	drawer.drawBoxFilled(xMin, yMin, xMax, yMax, color, rounding);
*	drawer.drawLine(xStart, yStart, xEnd, yEnd, color, thickness);
*	drawer.drawEllipse(xMin, yMin, xMax, yMax, color;
*	drawer.drawText(text, x, y, color);
*	drawer.drawTextCentered(text, x, y, color);
*	// logic end
*	
*	drawer.drawFrame();
* }
* 
* ~drawer;
* 
*/

class Drawer
{
public:
	HWND window{ nullptr };

	Drawer(float width, float height, UINT fps, HINSTANCE instance, INT cmdShow);
	~Drawer();

	// Pass your own function for quitting as the argument
	void initFrame(void (*quitFunction)());
	void drawFrame();

	void drawBox(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color, float rounding = 0.f, float thickness = 1.f);
	void drawBoxFilled(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color = ImColor(1.f, 1.f, 1.f), float rounding = 0);
	void drawLine(float xStart, float yStart, float xEnd, float yEnd, ImColor color = ImColor(1.f, 1.f, 1.f), float thickness = 1);
	void drawCircle(float xCenter, float yCenter, float radius, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawCircleFilled(float xCenter, float yCenter, float radius, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawText(char* text, float x, float y, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawTextCentered(char* text, float x, float y, ImColor color = ImColor(1.f, 1.f, 1.f));

	void setWindowClickable(bool clickable);


private:
	WNDCLASSEXW windowClass{};
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* deviceContext{ nullptr };
	IDXGISwapChain* swapChain{ nullptr };
	ID3D11RenderTargetView* renderTargetView{ nullptr };
	D3D_FEATURE_LEVEL level{};
	ID3D11Texture2D* backBuffer{ nullptr };

	ImDrawList* backgroundDrawList{ nullptr };

	WNDCLASSEXW makeWindowClass(const wchar_t* className, HINSTANCE applicationInstance);
	const HWND makeWindow(WNDCLASSEXW wc, HINSTANCE instance, float width, float height, const wchar_t* windowName);
	void adjustWindowRect(HWND window);
	DXGI_SWAP_CHAIN_DESC makeSwapChainDesc(HWND window, UINT targetfps);
};

void focusWindow(HWND hwnd);