
#include<DXg11/SceneManager.h>
#include"LifeCrystalUI.h"

#define SIZE_X (1200 * 0.5f)
#define SIZE_Y (70 * 0.7f)
#define POS_X (900 - SIZE_X / 2)
#define POS_Y (50)

#define DEC_SPED (0.01f)


LifeCrystalUI::LifeCrystalUI() {
	bar.Load("Assets/Textures/hp/crystalhp.png");
	bar.scale.x = SIZE_X;
	bar.scale.y = SIZE_Y;

	base.Load("Assets/Textures/hp/crystalbase.png");
	base.scale.x = SIZE_X;
	base.scale.y = SIZE_Y;
	bar.SetAnchor(Sprite::Center);
	base.SetAnchor(Sprite::Center);
	bar.pos = base.pos = Vector3(SCREEN_X / 2, POS_Y, 0.0f);

	life = maxLife = 1;
	oldNum = 1.0f;
	currentNum = 0.0f;
}


LifeCrystalUI::~LifeCrystalUI() {

}


void LifeCrystalUI::Update() {
	float rate = (float)life / (float)maxLife;

	if (rate < oldNum){
		oldNum = rate;
	}
	if (rate < currentNum) {
		currentNum -= DEC_SPED;
	}
	else {
		currentNum = rate;
	}

	bar.scale.x = SIZE_X  * currentNum;
}

void LifeCrystalUI::ScreenDraw(){
	if (GameObjectList::Get()->stopUpdate) return;
	if (SceneManager::Get()->GetCurrentScene() == SCENE_TITLE) return;

	base.Render();
	bar.Render();
}



UI_CrystalLife::UI_CrystalLife() {
	lifeUI = new LifeCrystalUI();
	GameObjectList::Get()->Generate(lifeUI);
}


UI_CrystalLife::~UI_CrystalLife() {
	lifeUI->Destroy();
	lifeUI = nullptr;
}

void UI_CrystalLife::SetMaxLife(int num, Vector3 pos) {
	lifeUI->maxLife = num; 
	lifeUI->base.pos = lifeUI->bar.pos = pos;


}
