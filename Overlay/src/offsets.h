#pragma once
#include <offsets/client_dll.hpp>
#include <offsets/offsets.hpp>

inline std::ptrdiff_t dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
inline std::ptrdiff_t m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
inline std::ptrdiff_t m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
inline std::ptrdiff_t m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
inline std::ptrdiff_t m_nSubclassID = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID;
inline std::ptrdiff_t m_hThrower = cs2_dumper::schemas::client_dll::C_BaseGrenade::m_hThrower;
inline std::ptrdiff_t m_flDamage = cs2_dumper::schemas::client_dll::C_BaseGrenade::m_flDamage;
inline std::ptrdiff_t m_vecOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecOrigin;
inline std::ptrdiff_t m_vecAbsOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
inline std::ptrdiff_t m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
inline std::ptrdiff_t dwViewMatrix = cs2_dumper::offsets::client_dll::dwViewMatrix;
inline std::ptrdiff_t m_bLeftHanded = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bLeftHanded;
inline std::ptrdiff_t dwLocalPlayerController = cs2_dumper::offsets::client_dll::dwLocalPlayerController;
inline std::ptrdiff_t dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
inline std::ptrdiff_t m_iszPlayerName = cs2_dumper::schemas::client_dll::CBasePlayerController::m_iszPlayerName;
inline std::ptrdiff_t m_pOwner = cs2_dumper::schemas::client_dll::CGameSceneNode::m_pOwner; // CEntityInstance*
inline std::ptrdiff_t m_pEntity = cs2_dumper::schemas::client_dll::CEntityInstance::m_pEntity; // CEntityIdentity*
inline std::ptrdiff_t m_designerName = cs2_dumper::schemas::client_dll::CEntityIdentity::m_designerName;
inline std::ptrdiff_t dwGlobalVars = cs2_dumper::offsets::client_dll::dwGlobalVars;
inline std::ptrdiff_t dwEntitySystem_highestEntityIndex = cs2_dumper::offsets::client_dll::dwGameEntitySystem_highestEntityIndex;
inline std::ptrdiff_t m_skeletonInstance = cs2_dumper::schemas::client_dll::CBodyComponentSkeletonInstance::m_skeletonInstance;
inline std::ptrdiff_t m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;