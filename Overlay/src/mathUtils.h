#pragma once
struct Vector3 {
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}  // Default constructor
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	// Basic operations
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
};

struct Vector4 {
	float x, y, z, w;

	Vector4() : x(0), y(0), z(0), w(0) {}  // Default constructor
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(Vector3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

	// Basic operations
	Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
};

struct Matrix4 {
	float m[16]{};

	Matrix4() : m() {
		for (int i = 0; i < 16; i++) {
			m[i] = 0.0f;
		}
	}
	Matrix4(float arr[16]) : m() {
		for (int i = 0; i < 16; i++) {
			m[i] = arr[i];
		}
	}

	// basic operations:
	float operator[](int index) const {
		if (index >= 0 && index < 16) {
			return m[index];
		}
		//throw std::out_of_range("Index out of range");
	}
	float& operator[](int index) {
		if (index >= 0 && index < 16) {
			return m[index];
		}
		//throw std::out_of_range("Index out of range");
	}
	Matrix4 operator+(const Matrix4& inputMatrix) const {
		Matrix4 out;
		for (int i = 0; i < 16; i++) {
			out[i] = m[i] + inputMatrix[i];
		}
		return out;
	}
	Matrix4 operator-(const Matrix4& inputMatrix) const {
		Matrix4 out;
		for (int i = 0; i < 16; i++) {
			out[i] = m[i] - inputMatrix[i];
		}
		return out;
	}
	Matrix4 operator*(const float scalar) const {
		Matrix4 out;

		for (int i = 0; i < 16; i++) {
			out[i] = m[i] * scalar;
		}
		return out;
	}

};

float distance(Vector3 begin, Vector3 end);

Vector3 lerp(Vector3 begin, Vector3 end, float t);
float lerp(float begin, float end, float t);
Vector3 normalize(Vector3 vec);