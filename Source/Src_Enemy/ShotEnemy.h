#pragma once
#include<DXg11/GameObjectList.h>
#include"EnemyBase.h"

// 目標を破壊
class ShotEnemy :public EnemyBase {
public:
	ShotEnemy();
	~ShotEnemy() {};

	void Start();
	void Update();
	void Draw();

void AddDamage(int num);

private:
	void Shot();
	void ManageBullet();

	// オブジェクト取得
	void AddProtectedObjData();

	// コライダ
	void Collide();


	int shotCnt;
	int intvDamage;
	DirectX::Vector3 *dest;	// 目的地
	const std::list<GameObject*> *protectedObjList;

};

