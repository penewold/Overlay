#include "mathUtils.h"
#include <math.h>

float distance(Vector3 begin, Vector3 end) {
	float dx = begin.x - end.x;
	float dy = begin.y - end.y;
	float dz = begin.z - end.z;

	float distance = sqrt(dx * dx + dy * dy + dz * dz);

	return distance;
}