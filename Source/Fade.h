// フェード イン/アウト (Fade.h)
#pragma once
#include <DirectX/DX11Polygon.h>
#include <DXg11/Sprite.h>
#include "DXg11/GameObjectBase.h"


class CScene;
class Fade:public GameObject
{


public:
	enum FADE {
	FADE_NONE = 0,		// 何もない状態
	FADE_IN,			// フェードイン処理
	FADE_OUT,			// フェードアウト処理
	FADE_END,
	FADE_MAX
};

	Fade();
	virtual ~Fade();
	
	void Init(void);	// initはGame.cppで呼び出す
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Set(FADE state);
 	static FADE Get() { return fade; }	// 終わったらFADE_END、リセットすることでFADE_NONEに
	static void StartFade();
	static void ResetFadeState();	// Fade初期状態に

private:
	static FADE fade;
	float fAlpha;
	Sprite sprite;
};

