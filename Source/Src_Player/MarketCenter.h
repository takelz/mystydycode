#pragma once

#include<DXg11/GameObjectList.h>
#include"ProtectedObject.h"

#define MAX_STORE_MENU_TEXT (3)

class StoreMenuText {
public:
	StoreMenuText();
	~StoreMenuText();
	void LoadSprite(Vector3 pos,const char* keyName,const char* texName);
	void ScreenDraw();
	void Rimit();
private:
	Sprite key;
	Sprite text;

};

class MarketCenter :public GameObject{
public:
	MarketCenter();
	~MarketCenter();
	void Start();
	void Update();
	void ScreenDraw();

	void OpenClose();
	void BuyPlayerPower(int *money);
	void BuyPlayerLife(int *money, int*life);
	void BuyTurretPower(int *money);
	void BuyBarrier(int  *money);

	void SetProtect(ProtectedObject *obj) { protect = obj; };
private:
	bool opened;
	ProtectedObject *protect;

	StoreMenuText storeMenuText[MAX_STORE_MENU_TEXT];

};



