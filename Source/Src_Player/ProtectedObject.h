#pragma once

#include<DXg11/GameObjectList.h>
#include"../Number.h"
#include<Model/ModelAnimator.h>
#include"Barrier.h"
#include"LifeCrystalUI.h"

class ProtectedObject :public GameObject {
public:
	ProtectedObject();
	~ProtectedObject() {};
	void Start();
	void Update();
	void Draw();
	void ScreenDraw();
	void AddDamage();

	bool isBarrier;
private:
	void EndGame();

	int life;


	UI_Number hpNum;
	UI_CrystalLife lifeUI;

	Barrier *barrier;

	ModelAnimator animator;

};



