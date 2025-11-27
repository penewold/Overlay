#include "mathUtils.h"


float distance(Vector3 begin, Vector3 end) {
	float dx = begin.x - end.x;
	float dy = begin.y - end.y;
	float dz = begin.z - end.z;

	float distance = sqrtf(dx * dx + dy * dy + dz * dz);

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

Vector4 multiplyMat4Vec4(Vector4 vec, Matrix4 mat) {
	Vector4 result;
	result.x = vec.x * mat[0] + vec.y * mat[1] + vec.z * mat[2] + vec.w * mat[3];
	result.y = vec.x * mat[4] + vec.y * mat[5] + vec.z * mat[6] + vec.w * mat[7];
	result.z = vec.x * mat[8] + vec.y * mat[9] + vec.z * mat[10] + vec.w * mat[11];
	result.w = vec.x * mat[12] + vec.y * mat[13] + vec.z * mat[14] + vec.w * mat[15];
	return result;
}

Vector2 worldToScreen(Vector3 worldPos, Matrix4 viewMatrix, Vector2 screenDimensions) {
	Vector4 transformed = multiplyMat4Vec4(Vector4(worldPos, 1.f), viewMatrix);
	Vector2 screenPos;

	if (transformed.w <= 0.0f) {
		return Vector2(-1, -1); // off-screen or invalid
	}

	// Perspective divide to get NDC
	Vector3 ndc;
	ndc.x = transformed.x / transformed.w;
	ndc.y = transformed.y / transformed.w;
	ndc.z = transformed.z / transformed.w;

	// Perspective divide
	if (transformed.w != 0) {
		screenPos.x = (ndc.x * 0.5f + 0.5f) * screenDimensions.x;
		screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * screenDimensions.y; // Flip Y
	}

	return screenPos;
}

Vector3 normalize(Vector3 vec)
{
	float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (length == 0) {
		return { 0, 0, 0 };
	}
	return { vec.x / length, vec.y / length, vec.z / length };
}

float lerp(float begin, float end, float t) {
	return begin + (end - begin) * t;
}

ImColor lerp(ImColor start, ImColor end, float t) {
	float r = lerp(start.Value.x, end.Value.x, t);
	float g = lerp(start.Value.y, end.Value.y, t);
	float b = lerp(start.Value.z, end.Value.z, t);
	return ImColor(r, g, b);
}
