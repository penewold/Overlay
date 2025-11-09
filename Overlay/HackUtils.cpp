#include "HackUtils.h"
#include "offsets/client_dll.hpp"
//#include "src/offsets.h"

HackUtils::HackUtils(memify& m) : mem(m) {
	entityList = 0;
}

void HackUtils::setEntList(uintptr_t listLocationPtr) {
	entityList = listLocationPtr;
}

uintptr_t HackUtils::getEntity1(int index) {
	uintptr_t ListEntry = mem.Read<uintptr_t>(entityList + (8 * (index & 0x7FFF) >> 9) + 16);
	if (!ListEntry) return 0;
	
	uintptr_t currentController = mem.Read<uintptr_t>(entityList + 0x10 + index * 0x70);
	uintptr_t playerController = mem.Read<uintptr_t>(ListEntry + 0x70 * (index & 0x1FF)); // C_BaseEntity?
	if (!playerController) return 0;

	if (currentController == 0) {
		return 0;
	}

	int pawnhandle = mem.Read<int>(currentController /* + m_hPlayerPawn*/);

	if (pawnhandle == 0) {
		return 0;
	}

	uintptr_t listEntry2 = mem.Read<uintptr_t>(entityList + 0x8 * ((pawnhandle & 0x7FFF) >> 9) + 0x10);
	
}

// Max entity index: 0x7FFF (32767)
constexpr uint32_t MAX_ENTITIES = 0x7FFF;

// Chunk size: 512 entities (2^9)
constexpr int CHUNK_SIZE = 1 << 9;  // 512
constexpr int CHUNK_SHIFT = 9;

// Offsets (CS2 - 2025)
constexpr uintptr_t OFFSET_CHUNK_ARRAY = 0x10;  // +16 from entityList
constexpr uintptr_t OFFSET_ENTITY_ENTRY = 0x70; // 112 decimal (changed in recent update)

// Resolve any entity (Controller or Pawn) by index using masks
uintptr_t HackUtils::getEntity2(int index)
{

	// entities are stored inside buckets of 512 entities
	// there are max 64 buckets

	// ands with this: 0000 0000 0000 0000 0111 1111 1111 1111 (0x7FFF)
	// so any number above 0x7FFF (32767) is just modulo to below
	// then it's shifted right 9 times. For example for 1067:
	// 0000 0100 0010 1011 becomes 0000 0000 0000 0010 so 2
	// the int is locked to below 32768 (1 << 15) so we can ignore everything but the right most 15 bits
	// 0000 010- ---- ---- or 000 0010 or 2
	// for 32766
	// 0111 1111 1111 1110
	// 0111 111- ---- ----
	// 63 in decimal

	uint32_t bucketIndex = (index & MAX_ENTITIES) >> 9;

	uintptr_t listEntry = mem.Read<uintptr_t>(entityList + (8 * bucketIndex) + 16);

	// just look at the rightmost 9 bits for the index in the bucket
	// for 32766
	// 0111 1111 1111 1110
	// do and with 0000 0001 1111 1111 (0x1FF)
	// becomes:
	// ---- ---1 1111 1110 (510)
	uint32_t indexInBucket = index & 0x1FF;
	// 112 (0x70) is the size of each entity. Was 120 (0x78) some time ago
	uintptr_t entityAddress = listEntry + 112 * indexInBucket;
	return mem.Read<uintptr_t>(entityAddress);

}
