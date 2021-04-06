// フェード イン/アウト (Fade.cpp)
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include "Fade.h"
#include "Game.h"

Fade::FADE Fade::fade;

namespace {
	const LPCSTR PATH_FADETEXTURE = "Assets/Textures/fade.png";
	const float FADE_TEXTURE_SIZE_X = 1.0f;
	const float FADE_TEXTURE_SIZE_Y = 1.0f;
	const float	FADE_RATE = 0.02f;	// フェード係数
	const float	FADE_RED = 0.0f;
	const float	FADE_GREEN = 0.0f;
	const float	FADE_BLUE = 0.0f;
};

// コンストラクタ
Fade::Fade(){

}

// デストラクタ
Fade::~Fade()
{
}

// 初期化
void Fade::Init(void)
{
	fAlpha = 1.0f;
	fade = FADE_IN;

	//polygon.SetTexture(PATH_FADETEXTURE);
	sprite.Load(PATH_FADETEXTURE);
	sprite.scale.x = SCREEN_X + 200;
	sprite.scale.y = SCREEN_Y;
	sprite.SetAnchor(Sprite::LeftTop);

}

// 終了処理
void Fade::Uninit(void)
{
	// (なし)
}

// 更新
void Fade::Update(void)
{
	if (fade != FADE_NONE || fade != FADE_END) {
		// フェード処理中
		if (fade == FADE_OUT) {
			// フェードアウト処理
			fAlpha += FADE_RATE;		// α値を加算して画面を消していく
			if (fAlpha >= 1.0f) {
				// フェードイン処理に切り替え
				fAlpha = 1.0f;
				fade = FADE_IN;
				// シーンを設定
				//CScene::Change(pNext);
			}
		} else if (fade == FADE_IN) {
			// フェードイン処理
			fAlpha -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
			if (fAlpha <= 0.0f) {
				// フェード処理終了
				fAlpha = 0.0f;
				fade = FADE_END;
			}
		}
	}
}

// 描画
void Fade::Draw(void)
{
	//DX11Graphics *pGraphics = GetDX11Graphics();
	//pGraphics->EnableZBuffer(false);
	//GetDX11Light()->SetEnable(false);
	//GetDX11Light()->Update();
	sprite.SetColor(FADE_RED, FADE_GREEN, FADE_BLUE, fAlpha);
	//polygon.Draw();
	sprite.Render();
}

// フェードの状態設定
void Fade::Set(FADE state)
{
	fade = state;

}

void Fade::StartFade() {
	fade = FADE_OUT;
}

void Fade::ResetFadeState() {
	fade = FADE_NONE;
}