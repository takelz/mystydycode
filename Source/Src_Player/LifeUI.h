#pragma once
#include"DXg11/GameObjectList.h"






class LifeUI:public GameObject {
public:

	LifeUI();
	~LifeUI();
	
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
	float redNum;
};






class UI_Life {
public:
	UI_Life();
	~UI_Life();

	void SetMaxLife(int num,Vector3 pos) ;
	void SetScale(Vector3 scale){ 	lifeUI->base.scale = lifeUI->bar.scale = scale; }
	void SetCurrentLife(int num) { lifeUI->life = num; };
	LifeUI *lifeUI;

};

