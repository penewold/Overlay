#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <iostream>

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

void drawCircle(ImDrawList* drawer, float x, float y, float radius, float width, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	drawer->AddCircle({ x, y }, radius, color, 0, width);
}

void drawBox(ImDrawList* drawer, float xMin, float yMin, float xMax, float yMax, float rounding = 0.f, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	drawer->AddRect(ImVec2(xMin, yMin), ImVec2(xMax, yMax), color, rounding);
}

void drawBoxFilled(ImDrawList* drawer, float xMin, float yMin, float xMax, float yMax, float rounding = 0.f, ImColor color = ImColor(1.f, 1.f, 1.f)) {
	drawer->AddRectFilled(ImVec2(xMin, yMin), ImVec2(xMax, yMax), color, rounding);
}
