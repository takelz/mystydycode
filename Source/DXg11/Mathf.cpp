#include"Mathf.h"
#include <DirectX/DX11Util.h>

using namespace DirectX;

namespace Mathf {

	Vector3 AngleYToVector(const float yAxis) {
		float radian = XMConvertToRadians(yAxis);
		float vecX = sinf(radian);
		float vecZ = cosf(radian);

		return Vector3(vecX, 0.0f, vecZ);
	}


	Vector3 AngleXToVector(const float xAxis){
		float radian = XMConvertToRadians(xAxis);
		float vecZ = cosf(radian);
		float vecY = sinf(radian);

		return Vector3(0.0f, vecY, vecZ);
	}

	float VectorToAngleY(const Vector3 vec) {
		float angle;
		float x = vec.z;
		float y = vec.x;
		//angle = acos(vec.x / sqrtf(vec.x * vec.x + vec.z * vec.z));
		//angle = angle * 180.0f / PI;
		//if (vec.z < 0) angle = 360.0f - angle;

		angle = atan2(y, x) * 180 / PI;

		return angle;
	}




	Vector3 DirectionVector2pt(Vector3 target, Vector3 pos) {
		return (target - pos).Normalize();
	}

	bool ComparePos(Vector3 posA, Vector3 posB, float dist) {
		if ((posA - posB).Length() < dist * dist) {
			return true;
		}
		return false;
	}

}