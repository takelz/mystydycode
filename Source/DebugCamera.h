#pragma once
#include "DXg11/GameObjectList.h"


class EditorControlCamera {
public:
	// x,y�̈ړ�
	void Move();
	void ClickMove();
	void Rotate();
	void ClickRotate();

private:
	// �N���b�N�����Ƃ����ۑ�
	Vector3 stackClickMovePos;
	Vector3 stackClickRotationPos;

};


class DebugCamera :public GameObject{
public:
	DebugCamera();
	~DebugCamera() {};

	void Update();
	void Draw();


	static Vector3 paramPosition;
	static Vector3 paramRotation;
	static Vector3 paramLookAt;
	static bool debugOn;
	static bool lookMode;				// true����LookAt�̈ʒu������
	

private:
	void SetData();
	void Move();
	EditorControlCamera controlCamera;

};

// �}�E�X���W�擾
Vector3 MousePosition();
