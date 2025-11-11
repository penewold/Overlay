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
	void fillEntityCache(uintptr_t list[1024], size_t size);
};

