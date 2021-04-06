#pragma once
#include"Vector.h"
#include<math.h>
#define PI (3.14159f)



namespace Mathf {

	// y����������x�N�g��xz�����߂�
	DirectX::Vector3 AngleYToVector(const float yAxis);

	// x����������x�N�g��yz�����߂�
	DirectX::Vector3 AngleXToVector(const float xAxis);

	// xz�������Ay�̊p�x�����߂�
	float VectorToAngleY(const DirectX::Vector3 vec);

	// ��_�Ԃ���target���ւ̌����x�N�g���𐳋K�����Ď擾
	DirectX::Vector3 DirectionVector2pt(DirectX::Vector3 target, DirectX::Vector3 pos);


	// ��_�Ԃ��w�苗���ȓ��ɓ�������true��Ԃ�
	bool ComparePos(DirectX::Vector3 posA, DirectX::Vector3 posB, float dist);
}
