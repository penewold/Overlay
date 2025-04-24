#pragma once
#include <offsets/client_dll.hpp>
#include <offsets/offsets.hpp>


std::ptrdiff_t dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
std::ptrdiff_t m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
std::ptrdiff_t m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
std::ptrdiff_t m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
std::ptrdiff_t m_nSubclassID = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID;
std::ptrdiff_t m_hThrower = cs2_dumper::schemas::client_dll::C_BaseGrenade::m_hThrower;
std::ptrdiff_t m_flDamage = cs2_dumper::schemas::client_dll::C_BaseGrenade::m_flDamage;
std::ptrdiff_t m_vecOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecOrigin;
std::ptrdiff_t m_vecAbsOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
std::ptrdiff_t m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
std::ptrdiff_t dwViewMatrix = cs2_dumper::offsets::client_dll::dwViewMatrix;
std::ptrdiff_t m_bLeftHanded = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bLeftHanded;
std::ptrdiff_t dwLocalPlayerController = cs2_dumper::offsets::client_dll::dwLocalPlayerController;
std::ptrdiff_t dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;