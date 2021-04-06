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
	// �I�u�W�F�N�g�擾
	void AddProtectedObjData();

	// �R���C�_
	void Collide();


	DirectX::Vector3 *dest;	// �ړI�n
	const std::list<GameObject*> *protectedObjList;

};

