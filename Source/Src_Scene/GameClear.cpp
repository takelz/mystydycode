
#include"GameClear.h"
#include"../Fade.h"
#include<DXg11/SceneManager.h>



#define WIDTH (1400)
#define HIEGHT (100)
#define POS_Y (370)

#define FADE_TIME (120)


GameClear::GameClear() {
	AddComponent(COMPONENT_SPRITE);
	GetSprite()->Load("Assets/textures/scene/clear.png");
	GetSprite()->SetAnchor(Sprite::Center);
	GetSprite()->scale = Vector3(WIDTH, HIEGHT, 0.0f);
	GetSprite()->pos.x = SCREEN_X/2;
	GetSprite()->pos.y = 0;
	lifeCnt = 0;
}

void GameClear::Update() {

	if (GetSprite()->pos.y < POS_Y) {
		GetSprite()->pos.y += 8;
	}
	lifeCnt++;

	if (lifeCnt > FADE_TIME) {
		// 暗く成りきってフェードインモードに切り替わったら
		if (Fade::Get() == Fade::FADE_IN) {
				SceneManager::Get()->LoadScene(SCENE_TITLE);
		}
		else {
			Fade::StartFade();
		}

	}
}

	void GameClear::ScreenDraw() {
		GetSprite()->Render();

	}