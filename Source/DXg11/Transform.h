#pragma once

#include"Vector.h"
#include<DirectXMath.h>

namespace DirectX {

	class Transform {
	public:
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		Transform();
		~Transform() {};
	
		// スケール値の一番大きい値を返す
		float GetHighestScale();
		// matrixを計算(LateUpdate後に自動的に実行)
		void UpdateMatrix();
		// GameObjectから自身に親を設定
		void ParentTransform(Transform *parent);
		// 親から接続解除
		void ParentDisconnect();

		int instanceID;		// ユニーク番号(数が若いほど最初に生成されたもの、つまりリストの先頭)

		DirectX::XMFLOAT4X4 GetMatrix() { UpdateMatrix(); return matrix; };
		Vector3 Forward();
		Vector3 Back();
		Vector3 Right();
		Vector3 Left();


	private:
		bool updatedMatrix;				// 変更がかかるとfalseに(アップデートされた後ならtrue)
		DirectX::XMFLOAT4X4 matrix;
		DirectX::Transform *parentTransform;

		// 1フレーム前の座標
		DirectX::Vector3 prevPosition;
		DirectX::Vector3 prevRotation;
		DirectX::Vector3 prevScale;

		//std::list<GameObject*> childList;

		// 
		void CompareParameter();
		

	};

}