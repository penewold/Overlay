#pragma once
#include "imgui/imgui.h"

struct AimbotSettings
{

};

struct SkeletonSettings
{
	bool doSkeletonEsp = true;
	ImColor skeletonColor = ImColor(1.f, 1.f, 1.f);
	bool doHeadEsp = true;
	ImColor headColor = ImColor(1.f, 1.f, 1.f);
	float headSize = 5.5f;
	bool doChickenSkeletonEsp = true;

};

struct BoxSettings {
	float boxRounding = 0.f;
	float boxThickness = 1.f;
	float playerHeight = 72.f;
	float ratio = 0.2f;
	ImColor boxColor = ImColor(1.f, 1.f, 1.f);

};

struct CheckSettings {
	bool doTeamCheck = true;
	bool doHealthCheck = true;
};

struct HealthSettings {
	ImColor healthStartColor = ImColor(0.f, 1.f, 0.f);
	ImColor healthEndColor = ImColor(1.f, 0.f, 0.f);
};

struct NameSettings
{
	ImColor nameColor = ImColor(1.f, 1.f, 1.f);
};

struct ESPSettings
{
	SkeletonSettings skeletonSettings;
	BoxSettings boxSettings;
	CheckSettings checkSettings;
	HealthSettings healthSettings;
	
	bool doBoxEsp = true;
	bool doSkeletonEsp = true;
	bool doHealthEsp = true;
	bool doNameEsp = true;
};

struct Settings {
	ESPSettings espSettings;
};