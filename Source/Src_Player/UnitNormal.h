#pragma once
#include"DXg11/GameObjectList.h"
#include"IDamageable.h"



class UnitNormal :public GameObject,public IDamageable{
public:
	UnitNormal();
	~UnitNormal() {};

	void Start();
	void Update();
	void Draw();

	void AddDamage();

	static int bulletAddPower;
	static int lifeAddPower;

private:
	// ��Ɏ����W����������
	RenderModel base;		// ���
	Transform baseTrans;

	void Shot();
	void ManageBullet();	// �e�����[���Ȃ����
	void End();


	int bulletNum;
	int shotCnt;
	int life;




};





