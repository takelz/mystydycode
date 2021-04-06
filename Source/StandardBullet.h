#pragma once
#include"DXg11/GameObjectList.h"

class StandardBullet :public GameObject {
public:
	StandardBullet();
	~StandardBullet() {};

	void Update();
	void Draw();

	Vector3 direction;
protected:
	void Move();
	void CollideObject();
	void End();
private:


	int endCnt;

};




