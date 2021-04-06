// �t�F�[�h �C��/�A�E�g (Fade.h)
#pragma once
#include <DirectX/DX11Polygon.h>
#include <DXg11/Sprite.h>
#include "DXg11/GameObjectBase.h"


class CScene;
class Fade:public GameObject
{


public:
	enum FADE {
	FADE_NONE = 0,		// �����Ȃ����
	FADE_IN,			// �t�F�[�h�C������
	FADE_OUT,			// �t�F�[�h�A�E�g����
	FADE_END,
	FADE_MAX
};

	Fade();
	virtual ~Fade();
	
	void Init(void);	// init��Game.cpp�ŌĂяo��
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Set(FADE state);
 	static FADE Get() { return fade; }	// �I�������FADE_END�A���Z�b�g���邱�Ƃ�FADE_NONE��
	static void StartFade();
	static void ResetFadeState();	// Fade������Ԃ�

private:
	static FADE fade;
	float fAlpha;
	Sprite sprite;
};

