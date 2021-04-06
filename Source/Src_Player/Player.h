#pragma once
#include "../DXg11/GameObjectList.h"
#include "../DebugCamera.h"
#include "WalletMoney.h"
#include "MarketCenter.h"
#include "UnitNormal.h"
#include "IDamageable.h"
#include "LifeUI.h"
#include "InputMove.h"
#include <Number.h>
#include "Barrier.h"


#include"../DebugCamera.h"

class Player :public GameObject,public IDamageable{
	struct Buffer {
		DirectX::XMFLOAT4 time;	//xÇÕéûä‘ÅAyÇÕÇµÇ´Ç¢íl
	};
public:
	Player();
	~Player() {};
	void Start();
	void Update();
	void Draw();
	void LateDraw();
	void ScreenDraw();
	void SetMarketCenter(MarketCenter* mkt) { marketCenter = mkt; };
	
	void AddDamage();

	Wallet wallet;
	UI_Life lifeUI;

	static bool atkPowerUp;
private:
	DebugCamera *camObj;
	MarketCenter *marketCenter;
	Barrier *barrier;
	InputMove inputMove;


	void Input();
	void Rotate();
	void FollowCamera();
	void DispUI();
	void Recovery();
	void BarrierSystem();

	float dash;
	int life;
	float damageColor;

	GraphicTexture randomGraphicTexture;
	ID3D11ShaderResourceView *pRandomTexture;
	ID3D11Buffer* constBuffer;
	Buffer setBuffer;
};


DebugCamera* CCC();