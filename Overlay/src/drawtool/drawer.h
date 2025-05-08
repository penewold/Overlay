#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <Windows.h>
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
	Drawer(float width, float height, UINT fps, HINSTANCE instance, INT cmdShow);
	~Drawer();

	void initFrame();
	void drawFrame();

	void drawBox(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color = ImColor(1.f, 1.f, 1.f), float rounding = 0);
	void drawBoxFilled(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color = ImColor(1.f, 1.f, 1.f), float rounding = 0);
	void drawLine(float xStart, float yStart, float xEnd, float yEnd, ImColor color = ImColor(1.f, 1.f, 1.f), float thickness = 1);
	void drawEllipse(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawEllipseFilled(float xMinimum, float yMinimum, float xMaximum, float yMaximum, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawText(char* text, float x, float y, ImColor color = ImColor(1.f, 1.f, 1.f));
	void drawTextCentered(char* text, float x, float y, ImColor color = ImColor(1.f, 1.f, 1.f));

private:

};

Drawer::Drawer(float width, float height, UINT fps, HINSTANCE instance, INT cmdShow)
{
}

Drawer::~Drawer()
{
}