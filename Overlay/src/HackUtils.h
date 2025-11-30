#pragma once
#include "memory/memify.h"
#include "mathUtils.h"
class HackUtils
{
private: 
	memify& mem;
public:
	uintptr_t entityList;

	HackUtils(memify& m);

	void setEntList(uintptr_t listLocationPtr);
	uintptr_t getEntity(int index);
	void fillEntityCache(uintptr_t list[], size_t size);
	uint8_t getEntityType(uintptr_t entityPtr);
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


