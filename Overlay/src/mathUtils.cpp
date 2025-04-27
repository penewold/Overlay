#include "mathUtils.h"
#include <math.h>

float distance(Vector3 begin, Vector3 end) {
	float dx = begin.x - end.x;
	float dy = begin.y - end.y;
	float dz = begin.z - end.z;

	float distance = sqrt(dx * dx + dy * dy + dz * dz);

	return distance;
}

Vector3 lerp(Vector3 begin, Vector3 end, float t)
{
	Vector3 result;
	result.x = begin.x + (end.x - begin.x) * t;
	result.y = begin.y + (end.y - begin.y) * t;
	result.z = begin.z + (end.z - begin.z) * t;

	return result;
}

Vector3 normalize(Vector3 vec)
{
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (length == 0) {
		return { 0, 0, 0 };
	}
	return { vec.x / length, vec.y / length, vec.z / length };
}