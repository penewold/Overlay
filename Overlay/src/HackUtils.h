#pragma once
#include "memory/memify.h"
#include "mathUtils.h"
#include "drawtool/drawer.h"
#include <vector>
#include "settings.h"

#ifndef ENT_CACHE_SIZE
#define ENT_CACHE_SIZE 1024
#endif

class HackUtils
{
private: 
	memify& mem;
	
public:
	uintptr_t entityList;
	uintptr_t entityListCache[ENT_CACHE_SIZE];
    //Drawer& drawer;

	HackUtils(memify& m);

	void setEntList(uintptr_t listLocationPtr);
	uintptr_t getEntity(int index);
	void fillEntityCache();
	uint8_t getEntityType(uintptr_t entityPtr);
	uintptr_t getCachedEntity(uint16_t index);
	std::vector<Vector3> getBones(uintptr_t gameSceneNode, uint16_t boneAmount);
	
    void drawHead(Drawer& drawer, std::vector<Vector3>& playerBones, ImColor headColor = ImColor(1.f, 1.f, 1.f), float headSize = 5.5f, float headThickness = 1.f);
    void drawSkeleton(Drawer& drawer, std::vector<Vector3>& playerBones, ImColor skeletonColor = ImColor(1.f, 1.f, 1.f), float boneThickness = 1.f);
    void drawChickenSkeleton(Drawer& drawer, std::vector<Vector3>& chickenBones, ImColor skeletonColor = ImColor(1.f, 1.f, 1.f), float boneThickness = 1.f);

    //int getEntityType(uintptr_t entityGameSceneNode);	
};

namespace entityTypes {
	enum entityTypes {
		PHYSICSPROP = 0,
		ANIMATEDPROP = 191,
		BOMBITEM = 47,
		PLANTSITE = 153,
		CHICKEN = 61,

	};

	enum weaponItems {
		KNIFEKNIFEITEM = 147,
		P2000GUNITEM = 199,
		P90GUNITEM = 148,
		NOVAGUNITEM = 116,
		GLOCKGUNITEM = 182,
		AK47ITEM = 4,
		XM1014ITEM = 73,
		SSG08ITEM = 238,
		DUALBERETTASITEM = 91,
		DEAGLEITEM = 135,
		P250ITEM = 132,
		GALILITEM = 163,
		MP5ITEM = 69,
		TEC9IEM = 16,
		MAC10ITEM = 19,
		SG = 553,
		AWP = 24,
	};

	enum grenades {
		HEGRENADE = 231,
		FLASHGRENADE = 70,
		SMOKEGRENADE = 192,
		MOLOTOVGRENADE = 206,
		DECOYGRENADE = 170,
	};

	enum grenadeItems {
		HEGRENADEITEM = 210,
		SMOKEGRENADEITEM = 168,
		FLASHGRENADEITEM = 49,
		MOLOTOVGRENADEITEM = 187,
		DECOYGRENADEITEM = 153,
	};


}

namespace skeletonData {
    enum boneIndex : uint8_t
    {
        pelvis = 0,
        spine_0 = 1,
        spine_1 = 2,
        spine_2 = 3,
        spine_3 = 4,
        neck_0 = 5,
        head_0 = 6,
        clavicle_l = 7,
        arm_upper_l = 8,
        arm_lower_l = 9,
        hand_l = 10,
        weapon_hand_l = 11,
        clavicle_r = 12,
        arm_upper_r = 13,
        arm_lower_r = 14,
        hand_r = 15,
        weapon_hand_r = 16,
        jiggle_primary = 17,
        chesthier_offset = 18,
        weaponhier_jnt = 19,
        weaponhier_r_iktarget = 20,
        weaponhier_l_iktarget = 21,
        leg_upper_l = 22,
        leg_lower_l = 23,
        ankle_l = 24,
        leg_upper_r = 25,
        leg_lower_r = 26,
        ankle_r = 27,
        root_motion = 28,
        leg_l_offset = 29,
        leg_l_iktarget = 30,
        leg_r_offset = 31,
        leg_r_iktarget = 32,
        eyeball_l = 33,
        eyeball_r = 34,
        eye_target = 35,
        head_0_twist = 36,
        arm_lower_l_twist = 37,
        arm_lower_l_twist1 = 38,
        finger_middle_meta_l = 39,
        finger_middle_0_l = 40,
        finger_middle_1_l = 41,
        finger_middle_2_l = 42,
        finger_pinky_meta_l = 43,
        finger_pinky_0_l = 44,
        finger_pinky_1_l = 45,
        finger_pinky_2_l = 46,
        finger_index_meta_l = 47,
        finger_index_0_l = 48,
        finger_index_1_l = 49,
        finger_index_2_l = 50,
        finger_thumb_0_l = 51,
        finger_thumb_1_l = 52,
        finger_thumb_2_l = 53,
        finger_ring_meta_l = 54,
        finger_ring_0_l = 55,
        finger_ring_1_l = 56,
        finger_ring_2_l = 57,
        arm_upper_l_twist1 = 58,
        arm_upper_l_twist = 59,
        pect_l_aimat = 60,
        scapula_l = 61,
        arm_lower_r_twist = 62,
        arm_lower_r_twist1 = 63,
        finger_middle_meta_r = 64,
        finger_middle_0_r = 65,
        finger_middle_1_r = 66,
        finger_middle_2_r = 67,
        finger_pinky_meta_r = 68,
        finger_pinky_0_r = 69,
        finger_pinky_1_r = 70,
        finger_pinky_2_r = 71,
        finger_index_meta_r = 72,
        finger_index_0_r = 73,
        finger_index_1_r = 74,
        finger_index_2_r = 75,
        finger_thumb_0_r = 76,
        finger_thumb_1_r = 77,
        finger_thumb_2_r = 78,
        finger_ring_meta_r = 79,
        finger_ring_0_r = 80,
        finger_ring_1_r = 81,
        finger_ring_2_r = 82,
        arm_upper_r_twist1 = 83,
        arm_upper_r_twist = 84,
        pect_r_aimat = 85,
        scapula_r = 86,
        pect_l_aimup = 87,
        pect_r_aimup = 88,
        scap_aimup = 89,
        pectaim_l = 90,
        pecttrans_l = 91,
        pectaim_r = 92,
        pecttrans_r = 93,
        scap_r_aimat = 94,
        scap_l_aimat = 95,
        pect_l_ptbase = 96,
        pect_r_ptbase = 97,
        ball_l = 98,
        leg_upper_l_twist = 99,
        leg_upper_l_twist1 = 100,
        ball_r = 101,
        leg_upper_r_twist = 102,
        leg_upper_r_twist1 = 103,
        feet_l = 104,
        feet_r = 109,
        knife_attachment = 114,
        main_weapon_attachment = 124,
        pistol_attachment = 113

    };

    static std::vector<std::pair<boneIndex, boneIndex>> boneConnections = {
        // Spine chain
        {boneIndex::pelvis,     boneIndex::spine_0},
        {boneIndex::spine_0,    boneIndex::spine_1},
        {boneIndex::spine_1,    boneIndex::spine_2},
        {boneIndex::spine_2,    boneIndex::spine_3},
        {boneIndex::spine_3,    boneIndex::neck_0},
        {boneIndex::neck_0,     boneIndex::head_0},

        // Left arm
        {boneIndex::spine_3,    boneIndex::clavicle_l},
        {boneIndex::clavicle_l, boneIndex::arm_upper_l},
        {boneIndex::arm_upper_l,boneIndex::arm_lower_l},
        {boneIndex::arm_lower_l,boneIndex::hand_l},

        // Right arm
        {boneIndex::spine_3,    boneIndex::clavicle_r},
        {boneIndex::clavicle_r, boneIndex::arm_upper_r},
        {boneIndex::arm_upper_r,boneIndex::arm_lower_r},
        {boneIndex::arm_lower_r,boneIndex::hand_r},

        // Left leg
        {boneIndex::pelvis,     boneIndex::leg_upper_l},
        {boneIndex::leg_upper_l,boneIndex::leg_lower_l},
        {boneIndex::leg_lower_l,boneIndex::ankle_l},

        // Right leg
        {boneIndex::pelvis,     boneIndex::leg_upper_r},
        {boneIndex::leg_upper_r,boneIndex::leg_lower_r},
        {boneIndex::leg_lower_r,boneIndex::ankle_r},

        // Hands
        {boneIndex::hand_l,     boneIndex::weapon_hand_l},
        {boneIndex::hand_r,     boneIndex::weapon_hand_r},
    };
}

namespace chickenSkeletonData {
    enum boneIndex : uint8_t {
        root = 0,
        body_0 = 1,
        body_1 = 2,
        body_2 = 3,
        collar_bone_0 = 4,
        breast_l = 5,
        wing_root_l = 6,
        wing_mid_l = 7,
        wing_tip_l = 8,
        neck_0 = 9,
        neck_1 = 10,
        neck_2 = 11,
        head_0 = 12,
        comb_front_0 = 13,
        comb_front_1 = 14,
        comb_back_0 = 15,
        comb_back_1 = 16,
        eye_l = 17,
        beak_root = 18,
        wattle_l_0 = 19,
        wattle_l_1 = 20,
        wattle_r_0 = 21,
        wattle_r_1 = 22,
        eye_r = 23,
        breast_r = 24,
        wing_root_r = 25,
        wing_mid_r = 26,
        wing_tip_r = 27,
        tail_0 = 28, 
        tail_1 = 29,
        leg_l_0 = 30,
        leg_l_1 = 31,
        leg_l_2 = 32,
        foot_l = 33,
        left_toe_l_0 = 34,
        left_toe_l_1 = 35,
        middle_toe_l_0 = 36,
        middle_toe_l_1 = 37,
        right_toe_l_0 = 38,
        right_toe_l_1 = 39,
        hallux_l = 40,
        leg_r_0 = 41,
        leg_r_1 = 42,
        leg_r_2 = 43,
        foot_r = 44,
        left_toe_r_0 = 45,
        left_toe_r_1 = 46,
        middle_toe_r_0 = 47,
        middle_toe_r_1 = 48,
        right_toe_r_0 = 49,
        right_toe_r_1 = 50,
        hallux_r = 51,
    };

    static std::vector<std::pair<boneIndex, boneIndex>> boneConnections = {
        // Spine chain
        {boneIndex::body_0,     boneIndex::collar_bone_0},
        {boneIndex::collar_bone_0,     boneIndex::breast_l},
        {boneIndex::breast_l,     boneIndex::wing_root_l},
        {boneIndex::wing_root_l,     boneIndex::wing_mid_l},
        {boneIndex::wing_mid_l,     boneIndex::wing_tip_l},

        {boneIndex::collar_bone_0,     boneIndex::neck_0},
        {boneIndex::neck_0,     boneIndex::neck_1},
        {boneIndex::neck_1,     boneIndex::neck_2},
        {boneIndex::neck_2,     boneIndex::head_0},
        {boneIndex::head_0,     boneIndex::comb_front_0},
        {boneIndex::comb_front_0,     boneIndex::comb_front_1},
        {boneIndex::head_0,     boneIndex::comb_back_0},
        {boneIndex::comb_back_0,     boneIndex::comb_back_1},
        {boneIndex::head_0,     boneIndex::eye_l},
        {boneIndex::head_0,     boneIndex::eye_r},
        {boneIndex::head_0,     boneIndex::beak_root},
        {boneIndex::beak_root,     boneIndex::wattle_l_0},
        {boneIndex::wattle_l_0,     boneIndex::wattle_l_1},
        {boneIndex::beak_root,     boneIndex::wattle_r_0},
        {boneIndex::wattle_r_0,     boneIndex::wattle_r_1},

        {boneIndex::collar_bone_0,     boneIndex::breast_r},
        {boneIndex::breast_r,     boneIndex::wing_root_r},
        {boneIndex::wing_root_r,     boneIndex::wing_mid_r},
        {boneIndex::wing_mid_r,     boneIndex::wing_tip_r},

        {boneIndex::body_0,     boneIndex::tail_0},
        {boneIndex::tail_0,     boneIndex::tail_1},

        {boneIndex::body_0,     boneIndex::leg_l_0},
        {boneIndex::leg_l_0,     boneIndex::leg_l_1},
        {boneIndex::leg_l_1,     boneIndex::leg_l_2},
        {boneIndex::leg_l_2,     boneIndex::foot_l},
        {boneIndex::foot_l,     boneIndex::left_toe_l_0},
        {boneIndex::left_toe_l_0,     boneIndex::left_toe_l_1},
        {boneIndex::foot_l,     boneIndex::middle_toe_l_0},
        {boneIndex::middle_toe_l_0,     boneIndex::middle_toe_l_1},
        {boneIndex::foot_l,     boneIndex::right_toe_l_0},
        {boneIndex::right_toe_l_0,     boneIndex::right_toe_l_1},
        {boneIndex::foot_l,     boneIndex::hallux_l},

        {boneIndex::body_0,     boneIndex::leg_r_0},
        {boneIndex::leg_r_0,     boneIndex::leg_r_1},
        {boneIndex::leg_r_1,     boneIndex::leg_r_2},
        {boneIndex::leg_r_2,     boneIndex::foot_r},
        {boneIndex::foot_r,     boneIndex::left_toe_r_0},
        {boneIndex::left_toe_r_0,     boneIndex::left_toe_r_1},
        {boneIndex::foot_r,     boneIndex::middle_toe_r_0},
        {boneIndex::middle_toe_r_0,     boneIndex::middle_toe_r_1},
        {boneIndex::foot_r,     boneIndex::right_toe_r_0},
        {boneIndex::right_toe_r_0,     boneIndex::right_toe_r_1},
        {boneIndex::foot_r,     boneIndex::hallux_r},
    };
}