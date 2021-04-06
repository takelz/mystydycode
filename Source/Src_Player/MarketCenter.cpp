
#include"MarketCenter.h"
#include<DirectX/DrawObject.h>
#include"Player.h"
#include"UnitNormal.h"

using namespace DirectX;

#define KEY_SPRITE_SIZE (300.0f * 0.4f)
#define TEXT_SPRITE_SIZE_X (1000.0f * 0.6f)
#define TEXT_SPRITE_SIZE_Y (300.0f * 0.6f)

#define OFFSET_KEY_TEXT_X (400.0f * 1.0f)
#define STORE_TEXT_POS_X (300.0f)
#define STORE_TEXT_POS_Y (170.0f)
#define STORE_TEXT_OFFSET_Y (150.0f)


#define BUY_PLAYERPOWER_MONEY (3)
#define BUY_HP_MONEY (3)
#define BUY_TURRETPOWER_MONEY (3)
#define BUY_BARRIER_MONEY (3)

#define DEFAULT_LIFE (7)


StoreMenuText::StoreMenuText() {
}

StoreMenuText::~StoreMenuText() {
}

void StoreMenuText::LoadSprite(Vector3 pos, const char* keyName, const char* texName){
	key.Load(keyName);
	text.Load(texName);
	key.pos = text.pos = pos;
	text.pos.x += OFFSET_KEY_TEXT_X;
	key.scale = KEY_SPRITE_SIZE;
	text.scale = Vector3(TEXT_SPRITE_SIZE_X, TEXT_SPRITE_SIZE_Y);

}

void StoreMenuText::ScreenDraw(){
	key.Render();
	text.Render();
}

void StoreMenuText::Rimit(){
	key.SetColor(0.5f, 0.1f, 0.1f, 1.0f);
	text.SetColor(0.5f, 0.1f, 0.1f, 1.0f);

}


// ç¿ïW0,0,0Ç…íuÇ≠

MarketCenter::MarketCenter() {
	storeMenuText[0].LoadSprite(Vector3(STORE_TEXT_POS_X, STORE_TEXT_POS_Y), 
			"Assets/Textures/store/key1.png", "Assets/Textures/store/textAttack.png");
	storeMenuText[1].LoadSprite(Vector3(STORE_TEXT_POS_X, STORE_TEXT_POS_Y + STORE_TEXT_OFFSET_Y),
		"Assets/Textures/store/key2.png", "Assets/Textures/store/textHp.png");
	storeMenuText[2].LoadSprite(Vector3(STORE_TEXT_POS_X, STORE_TEXT_POS_Y + STORE_TEXT_OFFSET_Y * 2.0f),
		"Assets/Textures/store/key3.png", "Assets/Textures/store/textTurret.png");
	opened = false;

	storeMenuText[0].Rimit();	// çUåÇÇæÇØñ¢é¿ëï
}


MarketCenter::~MarketCenter() {
}

void MarketCenter::Start() {
	 
}

void MarketCenter::Update(){

}

void MarketCenter::ScreenDraw(){
	if (opened) {
		for (int i = 0; i < MAX_STORE_MENU_TEXT; i++)
			storeMenuText[i].ScreenDraw();
	}
}


void MarketCenter::OpenClose() {
	opened = !opened;
}


void MarketCenter::BuyPlayerPower(int * money){
	if (*money < BUY_PLAYERPOWER_MONEY) {
		if (!opened) return;
		return;
	}

	*money -= BUY_PLAYERPOWER_MONEY;
	Player::atkPowerUp = true;

}

void MarketCenter::BuyPlayerLife(int * money, int * life){
	if (*money < BUY_HP_MONEY) {
		if (!opened) return;
		return;
	}
	if (*life >= DEFAULT_LIFE) return;
	*money -= BUY_HP_MONEY;
	*life += 1;


}

void MarketCenter::BuyTurretPower(int * money){
	if (*money < BUY_TURRETPOWER_MONEY) {
		if (!opened) return;
		return;
	}
	*money -= BUY_TURRETPOWER_MONEY;
	UnitNormal::bulletAddPower += 1;
	UnitNormal::lifeAddPower += 1;

}

void MarketCenter::BuyBarrier(int * money){
	if (*money < BUY_BARRIER_MONEY) {
		if (!opened) return;
		return;
	}
	*money -= BUY_BARRIER_MONEY;
	protect->isBarrier = true;
}


