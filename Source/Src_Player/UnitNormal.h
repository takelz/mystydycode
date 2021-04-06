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
	// 後に自動標準させたい
	RenderModel base;		// 台座
	Transform baseTrans;

	void Shot();
	void ManageBullet();	// 弾数がゼロなら消滅
	void End();


	int bulletNum;
	int shotCnt;
	int life;




};





