#pragma once
#include <imgui/imgui.h>

void drawText(ImDrawList* drawer, float x, float y, const char* text, ImColor color = ImColor(1.f, 1.f, 1.f));
void drawText(ImDrawList* drawer, float x, float y, int value, ImColor color = ImColor(1.f, 1.f, 1.f));
void drawCircle(ImDrawList* drawer, float x, float y, float radius, ImColor color = ImColor(1.f, 1.f, 1.f));
void drawCircle(ImDrawList* drawer, float x, float y, float radius, float width, ImColor color = ImColor(1.f, 1.f, 1.f));
void drawBox(ImDrawList* drawer, float xMin, float yMin, float xMax, float yMax, float rounding = 0.f, ImColor color = ImColor(1.f, 1.f, 1.f));
void drawBoxFilled(ImDrawList* drawer, float xMin, float yMin, float xMax, float yMax, float rounding = 0.f, ImColor color = ImColor(1.f, 1.f, 1.f));