#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <memory/memify.h>
#include "offsets.h"
#include "structs.h"
#include "mathUtils.h"
#include "logger.h"
#include "drawtool/drawer.h"
#include "HackUtils.h"
#include "tui.h"

#define ENT_COUNT 512
#define ENT_CACHE_SIZE 512
#define CACHE_REFRESH_RATE 1000

Vector2 screenDim(0, 0);

// Settings:
ImColor boxColor = ImColor(1.f, 1.f, 1.f);
ImColor healthStartColor = ImColor(0.f, 1.f, 0.f);
ImColor healthEndColor = ImColor(1.f, 0.f, 0.f);

bool doTeamCheck = true;
bool doHealthCheck = true;
bool doNameEsp = true;
int rainbowTime = 2000;
bool doRainbowBoxEsp = false;
bool doBoxEsp = true;
float boxRounding = 0.f;
float boxThickness = 1.f;
bool doHealthEsp = true;
bool doSkeletonEsp = true;

bool running = true;

bool showMenu = false;
bool menuDebounce = true;

HWND previousWindow = nullptr;

void exitApp() {
	running = false;
}



INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {

	#ifdef _DEBUG
	initLogger();
	enableAnsi();
	#endif

	screenDim.x = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	screenDim.y = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	Drawer drawer(screenDim.x, screenDim.y, 60, instance, cmd_show);
	memify mem("cs2.exe");
	uintptr_t client = mem.GetBase("client.dll");

	

	HackUtils hack(mem);

	uintptr_t entityList = mem.Read<uintptr_t>(client + dwEntityList);
	hack.setEntList(entityList);
	uintptr_t localPlayerPawn = 0;
	uintptr_t localPlayerGameSceneNode = 0;

	uint64_t currentTime = GetTickCount64();

	uint64_t lastEntityCacheTime = currentTime;
	hack.fillEntityCache();

	while (running) {
		

		if (!running) {
			break;
		}

		tui::resetScreen();
		drawer.initFrame(exitApp);
		currentTime = GetTickCount64();

		if ((currentTime - lastEntityCacheTime) > CACHE_REFRESH_RATE) {
			lastEntityCacheTime = currentTime;
			hack.fillEntityCache();

			localPlayerPawn = mem.Read<uintptr_t>(client + dwLocalPlayerPawn);
			localPlayerGameSceneNode = mem.Read<uintptr_t>(localPlayerPawn + m_pGameSceneNode);
		}

		std::cout << mem.Read<uint16_t>(client + dwEntitySystem_highestEntityIndex) << std::endl;

		Matrix4 viewMatrix = mem.Read<Matrix4>(client + dwViewMatrix);
		

		
		Vector3 localPlayerPos = mem.Read<Vector3>(localPlayerGameSceneNode + m_vecAbsOrigin);
		uint8_t localPlayerTeamNum = mem.Read<uint8_t>(localPlayerPawn + m_iTeamNum);

		ImColor rainbowColor = rainbowColor.HSV((GetTickCount64() % rainbowTime) / (float)rainbowTime, .8f, .9f);

		ImColor finalBoxColor = doRainbowBoxEsp ? rainbowColor : boxColor;

		int entcount1 = 0;

		

		for (int i = 0; i < ENT_COUNT; i++) {
			//uintptr_t currentEntity = hack.getEntity(i);
			uintptr_t currentEntity = hack.getCachedEntity(i);

			if (!currentEntity)
				continue;

			entcount1++;
			
			// put getEntityType here
			uintptr_t entityGameSceneNode = mem.Read<uintptr_t>(currentEntity + m_pGameSceneNode);
			int entityType = hack.getEntityType(entityGameSceneNode);

			Vector3 entityLocation = mem.Read<Vector3>(entityGameSceneNode + m_vecAbsOrigin);
			Vector2 entityScreenLocation = worldToScreen(entityLocation, viewMatrix, screenDim);

			std::string entityTypeStr = std::to_string(entityType);
			if (entityType == 61) {
				
				if (doSkeletonEsp) {
					std::vector<Vector3> chickenBones = hack.getBones(entityGameSceneNode, 54);

					for (auto bonePosition : chickenBones) {
						Vector2 screenPosition = worldToScreen(bonePosition, viewMatrix, screenDim);
						drawer.drawCircle(screenPosition, 2.f);
						
					}
				}
			}
			else {
				//drawer.drawTextCentered(entityTypeStr.c_str(), entityScreenLocation + Vector2(10.f, 0.f));
			}
			


			// Read the pawn handle (m_hPlayerPawn)
			// Assume they're a player
			int pawnHandle = mem.Read<int>(currentEntity + m_hPlayerPawn) & 0x7FFF;
			if (pawnHandle <= 0)  // 0 or negative = invalid
				continue;

			// Step 3: Get Player Pawn using the handle as entity index
			uintptr_t playerPawn = hack.getEntity(pawnHandle);
			if (!playerPawn)
				continue;

			// you can get the gamescenenode earlier without getting the playerpawn because the controller has the pointer to gamescenenode too
			uintptr_t pawnGameSceneNode = mem.Read<uintptr_t>(playerPawn + m_pGameSceneNode);
			int32_t health = mem.Read<int32_t>(playerPawn + m_iHealth);
			uint8_t teamNum = mem.Read<uint8_t>(playerPawn + m_iTeamNum);
			
			if (localPlayerPawn == playerPawn) continue;
			if (localPlayerTeamNum == teamNum && doTeamCheck) continue;
			if (health == 0 && doHealthCheck) continue;

			// esp
			Vector3 bottomPos = mem.Read<Vector3>(pawnGameSceneNode + m_vecAbsOrigin);
			Vector2 bottomScreenPos = worldToScreen(bottomPos, viewMatrix, screenDim);
			Vector3 topPos = bottomPos + Vector3(0.f, 0.f, 72.f);
			Vector2 topScreenPos = worldToScreen(topPos, viewMatrix, screenDim);
			//float distanceToLocalPlayer = distance(localPlayerPos, bottomPos);
			
			float width = (topScreenPos.y - bottomScreenPos.y) * 0.2f;

			if (doSkeletonEsp) {
				uintptr_t boneArrayPtr = mem.Read<uintptr_t>(pawnGameSceneNode + m_modelState + m_skeletonInstance);

				// basic bones are till 28. All are till 124
				std::vector<Vector3> playerBones = hack.getBones(pawnGameSceneNode, 28);
				int boneId = 0;
				for (auto bonePosition : playerBones) {
					Vector2 screenPosition = worldToScreen(bonePosition, viewMatrix, screenDim);
					if (boneId == 6) {
						Vector2 offsetScreenPosition = worldToScreen(bonePosition + Vector3(0, 0, 5.5f), viewMatrix, screenDim);
						drawer.drawCircle(screenPosition.x, screenPosition.y, distance(screenPosition, offsetScreenPosition));
					}
					/*drawer.drawTextCentered(std::to_string(boneId).c_str(), screenPosition.x, screenPosition.y);*/
					
					boneId++;
				}
			}

			if (doBoxEsp) {
				drawer.drawBox(bottomScreenPos.x - width, topScreenPos.y, bottomScreenPos.x + width, bottomScreenPos.y, finalBoxColor, boxRounding, boxThickness);
			}

			if (doHealthEsp) {
				ImColor healthBarColor = lerp(healthStartColor, healthEndColor, 1.f - (health / 100.f));
				float healthbarPos = lerp(bottomScreenPos.y, topScreenPos.y, health / 100.f);
				// xMin, yMin, xMax, yMax
				drawer.drawBoxFilled(bottomScreenPos.x - width - 3.f, bottomScreenPos.y, bottomScreenPos.x - width - 1.f, healthbarPos, healthBarColor, 0.f);
			}

			if (doNameEsp) {
				char playerName[128];
				mem.ReadRaw(currentEntity + m_iszPlayerName, playerName, sizeof(playerName));
				//drawer.drawTextCentered(playerName, bottomScreenPos.x, bottomScreenPos.y + 3.f);
				drawer.drawTextCentered(playerName, bottomScreenPos + Vector2(0, 3.f));
			}
			
		}

		std::cout << "ent count: " << tui::bold << entcount1 << "\n";
		if (showMenu) {
			ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
			ImGui::Begin("Overlay Menu", &showMenu);
			ImGui::Text("Esp Checks");
			ImGui::Checkbox("Team Check", &doTeamCheck);
			ImGui::Checkbox("Health Check", &doHealthCheck);

			ImGui::Text("Esp");
			ImGui::Checkbox("Box ESP", &doBoxEsp);
			if (doBoxEsp) {
				ImGui::Indent(20.f);
				ImGui::DragFloat("rounding", &boxRounding, 0.1f, 0.f, 10.f);
				ImGui::DragFloat("thickness", &boxThickness, 0.1f, 1.f, 10.f);
				ImGui::Checkbox("Rainbow Box ESP", &doRainbowBoxEsp);
				ImGui::ColorButton("##colorbox", finalBoxColor, 0, ImVec2(20, 20));

				if (!doRainbowBoxEsp) {
					ImGui::ColorPicker3("Box Color", (float*)&boxColor,
						ImGuiColorEditFlags_NoSidePreview |
						ImGuiColorEditFlags_NoInputs |
						ImGuiColorEditFlags_NoLabel);
				}
				else {
					ImGui::DragInt("Rainbow Time", &rainbowTime, 20.f, 100, 10000, "Time: %d ms");
				}
				ImGui::Unindent(20.f);
				
			}
			
			ImGui::Checkbox("Skeleton ESP", &doSkeletonEsp);
			if (doSkeletonEsp) {
				ImGui::Text("doing skeleton esp");
			}

			ImGui::Checkbox("Health ESP", &doHealthEsp);
			ImGui::Checkbox("Name ESP", &doNameEsp);
			ImGui::End();
		}
		else {
			drawer.setWindowClickable(false);
		}

		if (GetKeyState(VK_F8) & 0x8000) {
			if (!menuDebounce) {
				drawer.setWindowClickable(true);
				focusWindow(drawer.window);
				menuDebounce = true;
				showMenu = !showMenu;
			}
			else {
				focusWindow(mem.hwnd);
			}
		}
		else {
			menuDebounce = false;
		}
	
		drawer.drawFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
	}
	return 0;
}

