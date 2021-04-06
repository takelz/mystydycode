#pragma once
#include"Vector.h"
#include<math.h>
#define PI (3.14159f)



namespace Mathf {

	// y軸から向きベクトルxzを求める
	DirectX::Vector3 AngleYToVector(const float yAxis);

	// x軸から向きベクトルyzを求める
	DirectX::Vector3 AngleXToVector(const float xAxis);

	// xzを代入し、yの角度を求める
	float VectorToAngleY(const DirectX::Vector3 vec);

	// 二点間からtarget側への向きベクトルを正規化して取得
	DirectX::Vector3 DirectionVector2pt(DirectX::Vector3 target, DirectX::Vector3 pos);


	// 二点間が指定距離以内に入ったらtrueを返す
	bool ComparePos(DirectX::Vector3 posA, DirectX::Vector3 posB, float dist);
}
