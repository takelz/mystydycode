#pragma once
#include<DXg11/GameObjectList.h>
#include"EnemyBase.h"

// �ڕW��j��
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

	// �I�u�W�F�N�g�擾
	void AddProtectedObjData();

	// �R���C�_
	void Collide();


	int shotCnt;
	int intvDamage;
	DirectX::Vector3 *dest;	// �ړI�n
	const std::list<GameObject*> *protectedObjList;

};

