#pragma once
#include "DXg11/GameObjectList.h"


class EditorControlCamera {
public:
	// x,yの移動
	void Move();
	void ClickMove();
	void Rotate();
	void ClickRotate();

private:
	// クリックしたところを保存
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
	static bool lookMode;				// trueだとLookAtの位置を見る
	

private:
	void SetData();
	void Move();
	EditorControlCamera controlCamera;

};

// マウス座標取得
Vector3 MousePosition();
