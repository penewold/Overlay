#pragma once
#include "memory/memify.h"
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
		HEGRENADE = 231,
		FLASHGRENADE = 70,
		SMOKEGRENADE = 192,
		MOLOTOVGRENADE = 206,
		DECOYGRENADE = 170,
		HEGRENADEITEM = 210,
		SMOKEGRENADEITEM = 168,
		FLASHGRENADEITEM = 49,
		MOLOTOVGRENADEITEM = 187,
		DECOYGRENADEITEM = 153,
		NOVAGUNITEM = 116,
		GLOCKGUNITEM = 182,
		KNIFEKNIFEITEM = 147,
		BOMBITEM = 47,
		PLANTSITE = 153,
		P2000GUNITEM = 199,
		P90GUNITEM = 148,

	};

}

