#include "HackUtils.h"
#include "offsets/client_dll.hpp"
#include "src/offsets.h"

HackUtils::HackUtils(memify& m) : mem(m) {
	entityList = 0;
}

void HackUtils::setEntList(uintptr_t listLocationPtr) {
	entityList = listLocationPtr;
}

uintptr_t HackUtils::getEnt(int id) {
	uintptr_t ListEntry = mem.Read<uintptr_t>(entityList + (8 * (id & 0x7FFF) >> 9) + 16);
	if (!ListEntry) return 0;
	
	uintptr_t currentController = mem.Read<uintptr_t>(entityList + 0x10 + id * 0x70);
	uintptr_t playerController = mem.Read<uintptr_t>(ListEntry + 0x70 * (id & 0x1FF)); // C_BaseEntity?
	if (!playerController) return 0;

	if (currentController == 0) {
		return 0;
	}

	int pawnhandle = mem.Read<int>(currentController + m_hPlayerPawn);

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
constexpr uintptr_t OFFSET_ENTITY_ENTRY = 0x78; // 120 decimal

// Resolve any entity (Controller or Pawn) by index using masks
uintptr_t HackUtils::getEntity(int index)
{
    if (index < 0 || index > MAX_ENTITIES || !entityList)
        return 0;

    // Mask: lower 15 bits
    uint32_t id = index & 0x7FFF;

    // Chunk index: id >> 9
    uintptr_t chunk = mem.Read<uintptr_t>(
        entityList + OFFSET_CHUNK_ARRAY + 8 * (id >> CHUNK_SHIFT)
    );
    if (!chunk)
        return 0;

    // Local index in chunk: id & 0x1FF
    uintptr_t listEntry = mem.Read<uintptr_t>(
        chunk + OFFSET_ENTITY_ENTRY * (id & 0x1FF)
    );
    if (!listEntry)
        return 0;

    // Final entity pointer (Controller or Pawn)
    return mem.Read<uintptr_t>(listEntry + 0x10);
}