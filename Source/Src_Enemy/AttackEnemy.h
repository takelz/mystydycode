#pragma once
#include<DXg11/GameObjectList.h>

#include"StandardEnemy.h"

class AttackEnemy :public StandardEnemy {
public:
	AttackEnemy();
	~AttackEnemy() {};

	void Start();
	void Update();
	void Draw();



};

