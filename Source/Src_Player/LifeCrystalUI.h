#pragma once
#include"DXg11/GameObjectList.h"






class LifeCrystalUI:public GameObject {
public:

	LifeCrystalUI();
	~LifeCrystalUI();
	
	Sprite bar;
	Sprite base;

	int maxLife;
	int life;

	void Update();
	void ScreenDraw();
	void SetColor(float r,float g,float b) {
		bar.SetColor(r, g, b, 1.0f);
		base.SetColor(r, g, b, 1.0f);
	}

private:
	float oldNum;
	float currentNum;
};






class UI_CrystalLife {
public:
	UI_CrystalLife();
	~UI_CrystalLife();

	void SetMaxLife(int num,Vector3 pos) ;
	void SetCurrentLife(int num) { lifeUI->life = num; };
	LifeCrystalUI *lifeUI;

};

