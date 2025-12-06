#include "HackUtils.h"
#include "offsets/client_dll.hpp"
#include "offsets.h"

HackUtils::HackUtils(memify& m) : mem(m) {
	entityList = 0;
}

void HackUtils::setEntList(uintptr_t listLocationPtr) {
	entityList = listLocationPtr;
}

// Max entity index: 0x7FFF (32767)
constexpr uint32_t MAX_ENTITIES = 0x7FFF;

// Chunk size: 512 entities (2^9)
constexpr int CHUNK_SIZE = 1 << 9;  // 512
constexpr int CHUNK_SHIFT = 9;

// Offsets (CS2 - 2025)
constexpr uintptr_t OFFSET_CHUNK_ARRAY = 0x10;  // +16 from entityList
constexpr uintptr_t ENTITY_OFFSET = 0x70; // 112 decimal (changed in recent update)

// Resolve handle to entity
uintptr_t HackUtils::getEntity(int index)
{
	// entities are stored inside buckets of 512 entities
	// there are max 64 buckets

	// ands/masks with this: 0000 0000 0000 0000 0111 1111 1111 1111 (0x7FFF)
	// so any number above 0x7FFF (32767) is masked to something below
	// then it's shifted right 9 times. For example for 1067:
	// 0000 0100 0010 1011 becomes 0000 0000 0000 0010 so 2
	// the int is locked to below 32768 (1 << 15) so we can ignore everything but the right most 15 bits
	// 0000 010> >>>> >>>> or 000 0010 or 2
	// for 32766
	// 0111 1111 1111 1110
	// 0111 111- ---- ----
	// 63 in decimal

	uint32_t bucketIndex = (index & MAX_ENTITIES) >> 9;

	// pointer to the start of the bucket without the 16 bits that hold unknown data. vtable or smthn
	// each pointer to a bucket is 8 bits so thats why we * 8
	// 
	uintptr_t bucketPointer = mem.Read<uintptr_t>(entityList + (8 * bucketIndex) + 16);

	// just look at the rightmost 9 bits for the index in the bucket
	// for 32766
	// 0111 1111 1111 1110
	// do and with 0000 0001 1111 1111 (0x1FF)
	// becomes:
	// ---- ---1 1111 1110 (510)
	// index is the 9 rightmost bits
	uint32_t indexInBucket = index & 0x1FF;
	// 112 (0x70) is the size of each entity. Was 120 (0x78) some time ago
	uintptr_t entityAddress = bucketPointer + ENTITY_OFFSET * indexInBucket;
	return mem.Read<uintptr_t>(entityAddress);

}

void HackUtils::fillEntityCache() {
	for (int index = 0; index < sizeof(entityListCache) / sizeof(uintptr_t); index++) {
		entityListCache[index] = getEntity(index);
	}
}

uintptr_t HackUtils::getCachedEntity(uint16_t index) {
	if (index < 0) return 0;
	if (index >= ENT_CACHE_SIZE) return getEntity(index);
	return entityListCache[index];
}

/*int HackUtils::getEntityType(uintptr_t entityPtr) {
	uintptr_t entityGameSceneNode = mem.Read<uintptr_t>(entityPtr + m_pGameSceneNode);
	uintptr_t entityInstance = mem.Read<uintptr_t>(entityGameSceneNode + m_pOwner);
	if (!entityInstance) return -1;
	uintptr_t entityIdentity = mem.Read<uintptr_t>(entityInstance + m_pEntity);
	if (!entityIdentity) return -1;
	int designerName = mem.Read<int>(entityIdentity + m_designerName);

	return designerName;
}*/

uint8_t HackUtils::getEntityType(uintptr_t entityGameSceneNode) {
	uintptr_t entityInstance = mem.Read<uintptr_t>(entityGameSceneNode + m_pOwner);
	if (!entityInstance) return -1;
	uintptr_t entityIdentity = mem.Read<uintptr_t>(entityInstance + m_pEntity);
	if (!entityIdentity) return -1;
	uint8_t designerName = mem.Read<int>(entityIdentity + m_designerName);

	return designerName;
}

std::vector<Vector3> HackUtils::getBones(uintptr_t gameSceneNode, uint16_t boneAmount) {
	uintptr_t boneArrayPtr = mem.Read<uintptr_t>(gameSceneNode + m_modelState + m_skeletonInstance);

	std::vector<Vector3> bones;
	bones.reserve(boneAmount);
	if (boneArrayPtr) {
		// flip this if later when in seperate function
		for (int boneId = 0; boneId < boneAmount; boneId++) {
			Vector3 bonePos = mem.Read<Vector3>(boneArrayPtr + boneId * 0x20);
			bones.push_back(bonePos);
		}

	}
	return bones;
}