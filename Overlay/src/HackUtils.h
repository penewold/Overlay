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
	uintptr_t getEntity1(int index);
	uintptr_t getEntity2(int index);
};

