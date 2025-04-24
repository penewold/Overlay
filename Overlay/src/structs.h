#pragma once
#include "mathUtils.h"

#pragma pack(push, 1)
struct CGameSceneNode {
	char _pad0[0x10]; // Unknown
	char m_nodeToWorld[0x20]; // CTransform
    char m_pOwner[0x8]; // CEntityInstance*
    char m_pParent[0x8]; // CGameSceneNode*
    char m_pChild[0x8]; // CGameSceneNode*
    char m_pNextSibling[0x30]; // CGameSceneNode*
    char m_hParent[0xF]; // CGameSceneNodeHandle
    char m_vecOrigin[0x38]; // CNetworkOriginCellCoordQuantizedVector
    char m_angRotation[0x0C]; // QAngle
    char m_flScale[0x4]; // float32
    Vector3 m_vecAbsOrigin; // Vector
};