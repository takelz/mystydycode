
#include<DXg11/SceneManager.h>
#include"LifeUI.h"

#define SIZE_X (800* 0.7f)
#define SIZE_Y (100* 0.7f)
#define POS_X (900 - SIZE_X / 2)
#define POS_Y (50)

#define DEC_SPED (0.008f)
#define DEC_RED (0.01f)

LifeUI::LifeUI(){
	bar.Load("Assets/Textures/hp/hpgage.png");
	bar.scale.x = SIZE_X;
	bar.scale.y = SIZE_Y;

	base.Load("Assets/Textures/hp/hpbase.png");
	base.scale.x = SIZE_X;
	base.scale.y = SIZE_Y;
	bar.SetAnchor(Sprite::Center);
	base.SetAnchor(Sprite::Center);
	bar.pos = base.pos = Vector3(SCREEN_X/2, POS_Y, 0.0f);

	life = maxLife = 1;
	oldNum = 1.0f;
	currentNum = 0.0f;
}


LifeUI::~LifeUI() {
	
}


void LifeUI::Update() {
	float rate = (float)life / (float)maxLife;

	if (rate < oldNum) {
		oldNum = rate;
	}
	if (rate < currentNum) {
		currentNum -= DEC_SPED;
	}
	else {
		currentNum = rate;
	}

	if (redNum > 0.0f) redNum -= DEC_RED;

	bar.scale.x = SIZE_X * currentNum;
}

void LifeUI::ScreenDraw(){
	if (GameObjectList::Get()->stopUpdate) return;
	if (SceneManager::Get()->GetCurrentScene() == SCENE_TITLE) return;
	bar.SetColor(1.0f, 1.0f ,1.0f , 1.0f);
	bar.Render();
	base.Render();

}



UI_Life::UI_Life() {
	lifeUI = new LifeUI();
	GameObjectList::Get()->Generate(lifeUI);
}


UI_Life::~UI_Life() {
	lifeUI->Destroy();
	lifeUI = nullptr;
}

void UI_Life::SetMaxLife(int num, Vector3 pos) {
	lifeUI->maxLife = num; 
	lifeUI->base.pos = lifeUI->bar.pos = pos;


}
