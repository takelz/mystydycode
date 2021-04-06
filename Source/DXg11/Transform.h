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
	
		// �X�P�[���l�̈�ԑ傫���l��Ԃ�
		float GetHighestScale();
		// matrix���v�Z(LateUpdate��Ɏ����I�Ɏ��s)
		void UpdateMatrix();
		// GameObject���玩�g�ɐe��ݒ�
		void ParentTransform(Transform *parent);
		// �e����ڑ�����
		void ParentDisconnect();

		int instanceID;		// ���j�[�N�ԍ�(�����Ⴂ�قǍŏ��ɐ������ꂽ���́A�܂胊�X�g�̐擪)

		DirectX::XMFLOAT4X4 GetMatrix() { UpdateMatrix(); return matrix; };
		Vector3 Forward();
		Vector3 Back();
		Vector3 Right();
		Vector3 Left();


	private:
		bool updatedMatrix;				// �ύX���������false��(�A�b�v�f�[�g���ꂽ��Ȃ�true)
		DirectX::XMFLOAT4X4 matrix;
		DirectX::Transform *parentTransform;

		// 1�t���[���O�̍��W
		DirectX::Vector3 prevPosition;
		DirectX::Vector3 prevRotation;
		DirectX::Vector3 prevScale;

		//std::list<GameObject*> childList;

		// 
		void CompareParameter();
		

	};

}