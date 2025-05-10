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
#include "logger.h"
#include "drawtool/drawer.h"

Vector2 screenDim(0, 0);

// Settings:
ImColor boxColor = ImColor(1.f, 0.1f, 0.1f);
ImColor healthStartColor = ImColor(0.f, 1.f, 0.f);
ImColor healthEndColor = ImColor(1.f, 0.f, 0.f);

bool doTeamCheck = true;
bool doHealthCheck = true;
bool doNameEsp = true;
bool doRainbowBoxEsp = true;

void exitApp() {
	PostQuitMessage(0);
}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {

	#ifdef _DEBUG
	initLogger();
	#endif

	screenDim.x = GetSystemMetrics(SM_CXSCREEN);
	screenDim.y = GetSystemMetrics(SM_CYSCREEN);

	Drawer drawer(screenDim.x, screenDim.y, 60, instance, cmd_show);

	memify mem("cs2.exe");

	uintptr_t client = mem.GetBase("client.dll");

	bool running = true;

	while (running) {
		drawer.initFrame(exitApp);

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
			uint8_t teamNum = mem.Read<uint8_t>(currentPawn + m_iTeamNum);
			CGameSceneNode gameSceneNodeStruct = mem.Read<CGameSceneNode>(gameSceneNode);

			uintptr_t localPlayerPawn = mem.Read<uintptr_t>(client + dwLocalPlayerPawn);
			uintptr_t localPlayerGameSceneNode = mem.Read<uintptr_t>(localPlayerPawn + m_pGameSceneNode);
			Vector3 localPlayerPos = mem.Read<Vector3>(localPlayerGameSceneNode + m_vecAbsOrigin);
			uint8_t localPlayerTeamNum = mem.Read<uint8_t>(localPlayerPawn + m_iTeamNum);
			
			if (localPlayerPawn == currentPawn) continue;
			if (localPlayerTeamNum == teamNum && doTeamCheck) continue;
			if (health == 0 && doHealthCheck) continue;
			// Make a box around the player
			Vector3 bottomPos = mem.Read<Vector3>(gameSceneNode + m_vecAbsOrigin);
			Vector2 bottomScreenPos = worldToScreen(bottomPos, viewMatrix, screenDim);
			Vector3 topPos = bottomPos + Vector3(0.f, 0.f, 72.f);
			Vector2 topScreenPos = worldToScreen(topPos, viewMatrix, screenDim);
			float distanceToLocalPlayer = distance(localPlayerPos, bottomPos);
			float width = 10000 / distanceToLocalPlayer;

			ImColor rainbowColor;
			int rainbowTime = 2000;
			rainbowColor = rainbowColor.HSV((GetTickCount() % rainbowTime) / (float)rainbowTime, .8f, .9f);
			drawer.drawBox(bottomScreenPos.x - width, topScreenPos.y, bottomScreenPos.x + width, bottomScreenPos.y, rainbowColor, 2.f);

			ImColor healthBarColor = lerp(healthStartColor, healthEndColor, 1.f - (health / 100.f));
			float healthbarPos = lerp(bottomScreenPos.y, topScreenPos.y, health / 100.f);
			// xMin, yMin, xMax, yMax
			drawer.drawBoxFilled(bottomScreenPos.x - width - 3.f, bottomScreenPos.y, bottomScreenPos.x - width - 1.f, healthbarPos, healthBarColor, 0.f);

			if (!doNameEsp) continue;
			char playerName[128];
			mem.ReadRaw(currentController + m_iszPlayerName, playerName, sizeof(playerName));
			drawer.drawText(playerName, bottomScreenPos.x, bottomScreenPos.y);
		}
	
		drawer.drawFrame();
	}
	return 0;
}