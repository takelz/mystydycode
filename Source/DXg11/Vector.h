#pragma once
#include<math.h>

namespace DirectX {

	class Vector3 {
	public:
		float x, y, z;

		Vector3() { x = 0.0f;y = 0.0f; z = 0.0f; };
		Vector3(float nX, float nY, float nZ) { x = nX; y = nY; z = nZ; };
		Vector3(float nX, float nY) { x = nX; y = nY; z = 0.0f; };
		Vector3(const Vector3& nV) { x = nV.x; y = nV.y; z = nV.z; };
		~Vector3() {};

		inline Vector3 operator +(Vector3 & r) {
			return Vector3(x = x + r.x, y = y + r.y, z = z + r.z);
		}

		inline Vector3 operator -(Vector3 & r) {
			return Vector3(x = x - r.x, y = y - r.y, z = z - r.z);
		}

		inline Vector3 operator *(Vector3 & r) {
			return Vector3(x = x * r.x, y = y * r.y, z = z * r.z);
		}

		inline Vector3 operator /(Vector3 & r) {
			return Vector3(x = x / r.x, y = y / r.y, z = z / r.z);
		}

		inline Vector3 operator  *(float r) const {
			return Vector3(x * r, y * r, z * r);
		}

		inline Vector3 operator  /(float r) const {
			return Vector3(x / r, y / r, z / r);
		}


		inline Vector3 operator  =(float r) {
			x = r; y = r; z = r;
			return *this;
		}

		inline Vector3 operator  +=(float r) {
			x += r; y += r; z += r;
			return *this;
		}

		inline Vector3 operator  -=(float r) {
			x -= r; y -= r; z -= r;
			return *this;
		}

		inline Vector3 operator  *=(float r) {
			x *= r; y *= r; z *= r;
			return *this;
		}

		inline Vector3 operator  /=(float r) {
			x /= r; y /= r; z /= r;
			return *this;
		}

		inline Vector3 operator +=(const Vector3 & r) {
			return Vector3(x = x + r.x, y = y + r.y, z = z + r.z);
		}

		inline Vector3 operator -=(const Vector3 & r) {
			return Vector3(x = x - r.x, y = y - r.y, z = z - r.z);
		}

		inline Vector3 operator *=(const Vector3 & r) {
			return Vector3(x = x * r.x, y = y * r.y, z = z * r.z);
		}

		inline Vector3 operator /=(const Vector3 & r) {
			return Vector3(x = x / r.x, y = y / r.y, z = z / r.z);
		}
		// è≠êîàÍåÖÇ‹Ç≈î‰är
		inline bool operator ==(const Vector3 &r) {
			if (roundf(x * 10) == roundf(r.x * 10) && 
				roundf(y * 10) == roundf(r.y * 10) && 
				roundf(z * 10) == roundf(r.z * 10)) 
				return true;	
			else return false;
		}

		void Zero() {
			x = 0.0f; y = 0.0f; z = 0.0f;
		}

		float Length() {
			return sqrtf(x * x + y * y + z * z);
		}

		Vector3 Normalize() {
			return Vector3(x,y,z) / Length();
		}

	private:
	};



}