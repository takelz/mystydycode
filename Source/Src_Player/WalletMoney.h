#pragma once

#include<DXg11/GameObjectList.h>
#include"../Number.h"

class Coin :public GameObject {
public:
	Coin();
	~Coin();
	void Draw();
	void Update();

private:
	bool picked;
	int lifeTime;
	float colorAlpha;
};

class Wallet {
public:
	Wallet();
	~Wallet();



	void UseMoney(int num);	
	void AddMoney(int num);
	void ScreenDraw();
	int GetMoney() { return money; };

	int money;
private:
	UI_Number uiWalletNum;
	Sprite coinTex;

};



