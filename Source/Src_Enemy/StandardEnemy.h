#pragma once
#include<DXg11/GameObjectList.h>
#include"EnemyBase.h"


class StandardEnemy :public EnemyBase {
public:
	StandardEnemy();
	~StandardEnemy() {};

	void Start();
	void Update();
	void Draw();

void AddDamage(int num);

protected:
	// オブジェクト取得
	void AddProtectedObjData();

	// コライダ
	void Collide();


	DirectX::Vector3 *dest;	// 目的地
	const std::list<GameObject*> *protectedObjList;

};

