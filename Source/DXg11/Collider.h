#pragma once

#include"Transform.h"
#include<vector>

// 仕様大幅変更の可能性あり
// マトリックスはUpdate内で常に更新するように各オブジェクトで書く
// 現状二つ以上重なるとうまく動かない
// retCollをリストで返す

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

	// 必要な情報をセットする、これをしないとストップする
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

	// ＝＝＝＝＝変数＝＝＝＝＝
	GameObject *retColl;				// 最後当たった相手のコライダ
	std::vector<GameObject*> retCollList;// 	
	
	DirectX::Vector3 center;		// 中心。基本的には座標かモデルの中心
	DirectX::Transform *transform;	// 
	float radius;
	DirectX::Vector3 collBox;		// コライダ
	DirectX::XMFLOAT4X4 *mtx;

	bool completeSetup;
	int maxCollide;					// 最大でいくつ同時判定をとるか


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