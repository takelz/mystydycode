
#include"WaveUI.h"
#include<Shader/PostPerlinGlass.h>

#define WIDTH (1000 *0.7)
#define HEIGHT (150 * 0.5)
#define ALPHA_SPD (0.02f)

WaveUI::WaveUI(const char * name) {
	AddComponent(COMPONENT_SPRITE);
	GetSprite()->Load(name);
	GetSprite()->SetAnchor(Sprite::Center);
	GetSprite()->scale = Vector3(WIDTH, HEIGHT, 0.0f);
	GetSprite()->pos.x = SCREEN_X / 2;
	posX = SCREEN_X / 2;
	GetSprite()->pos.y = SCREEN_Y/ 2;
	alpha = 0.0f;
	GetSprite()->SetColor(1.0f,1.0f,1.0f,alpha);

}


void WaveUI::Update() {
	if (alpha < 1.0f) {
		alpha += ALPHA_SPD;
	}
	else {
		alpha = 1.0f;
		posX -= 14.0f;
		GetSprite()->pos.x = posX;
	}

	GetSprite()->SetColor(1.0f, 1.0f, 1.0f, alpha);

	if (posX <= -1000) Destroy();
}

void WaveUI::ScreenDraw() {

	GetSprite()->Render();
}

void WaveUI::LateUpdate() {

}