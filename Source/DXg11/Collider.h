#pragma once

#include"Transform.h"
#include<vector>

// �d�l�啝�ύX�̉\������
// �}�g���b�N�X��Update���ŏ�ɍX�V����悤�Ɋe�I�u�W�F�N�g�ŏ���
// �����ȏ�d�Ȃ�Ƃ��܂������Ȃ�
// retColl�����X�g�ŕԂ�

#define MAX_COLLIDE_NUM (5)

class GameObject;

class ColliderBase {

public:
	enum COLLIDER_TYPE {
		COLLIDER_CIRCLE,
		COLLIDER_AABB,
		COLLIDER_OBB,
	};

	ColliderBase() {
	completeSetup = false; 
	transform = nullptr;
	radius = 0;
	collBox = 0;
	mtx = nullptr;
	completeSetup = false;
	retColl = nullptr;
	maxCollide = MAX_COLLIDE_NUM;

	}
	virtual ~ColliderBase() {};

	// �K�v�ȏ����Z�b�g����A��������Ȃ��ƃX�g�b�v����
	void SetupStatus(DirectX::Vector3 inCenter,DirectX::Transform* inTrans,float inRad,
						DirectX::Vector3 inCollBox,DirectX::XMFLOAT4X4* inMtx) {
		center = inCenter;
		transform = inTrans;
		radius = inRad;
		collBox = inCollBox;
		mtx = inMtx;
		completeSetup = true;
	};

	COLLIDER_TYPE GetType() { return type; };

	virtual bool Collide() = 0;

	// �����������ϐ�����������
	GameObject *retColl;				// �Ō㓖����������̃R���C�_
	std::vector<GameObject*> retCollList;// 	
	
	DirectX::Vector3 center;		// ���S�B��{�I�ɂ͍��W�����f���̒��S
	DirectX::Transform *transform;	// 
	float radius;
	DirectX::Vector3 collBox;		// �R���C�_
	DirectX::XMFLOAT4X4 *mtx;

	bool completeSetup;
	int maxCollide;					// �ő�ł�������������Ƃ邩


protected:
	COLLIDER_TYPE type;

};

class ColliderCircle :public ColliderBase {
public:
	ColliderCircle();
	~ColliderCircle();
	bool Collide();


private:

};

class ColliderAABB :public ColliderBase {
public:
	ColliderAABB();
	~ColliderAABB();
	bool Collide();

private:

};

class ColliderOBB :public ColliderBase {
public:
	ColliderOBB();
	~ColliderOBB();
	bool Collide();


private:

};



class ColliderRay :public ColliderBase {
public:
	ColliderRay();
	~ColliderRay();
private:

};