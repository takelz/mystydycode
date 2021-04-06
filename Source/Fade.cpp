// �t�F�[�h �C��/�A�E�g (Fade.cpp)
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include "Fade.h"
#include "Game.h"

Fade::FADE Fade::fade;

namespace {
	const LPCSTR PATH_FADETEXTURE = "Assets/Textures/fade.png";
	const float FADE_TEXTURE_SIZE_X = 1.0f;
	const float FADE_TEXTURE_SIZE_Y = 1.0f;
	const float	FADE_RATE = 0.02f;	// �t�F�[�h�W��
	const float	FADE_RED = 0.0f;
	const float	FADE_GREEN = 0.0f;
	const float	FADE_BLUE = 0.0f;
};

// �R���X�g���N�^
Fade::Fade(){

}

// �f�X�g���N�^
Fade::~Fade()
{
}

// ������
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

// �I������
void Fade::Uninit(void)
{
	// (�Ȃ�)
}

// �X�V
void Fade::Update(void)
{
	if (fade != FADE_NONE || fade != FADE_END) {
		// �t�F�[�h������
		if (fade == FADE_OUT) {
			// �t�F�[�h�A�E�g����
			fAlpha += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
			if (fAlpha >= 1.0f) {
				// �t�F�[�h�C�������ɐ؂�ւ�
				fAlpha = 1.0f;
				fade = FADE_IN;
				// �V�[����ݒ�
				//CScene::Change(pNext);
			}
		} else if (fade == FADE_IN) {
			// �t�F�[�h�C������
			fAlpha -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if (fAlpha <= 0.0f) {
				// �t�F�[�h�����I��
				fAlpha = 0.0f;
				fade = FADE_END;
			}
		}
	}
}

// �`��
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

// �t�F�[�h�̏�Ԑݒ�
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