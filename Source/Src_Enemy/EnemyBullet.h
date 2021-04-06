#pragma once
#include"DXg11/GameObjectList.h"
#include"../StandardBullet.h"

class EnemyBullet :public StandardBullet {
public:
	EnemyBullet();
	~EnemyBullet() {};

	void Update();


protected:

private:


	int endCnt;

};




