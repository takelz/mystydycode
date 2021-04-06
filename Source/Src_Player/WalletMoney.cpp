
#include"WalletMoney.h"
#include<DirectX/DrawObject.h>
#include"Player.h"
#include<DXg11/SceneManager.h>

#define ADD_NUM (1)
#define PICKED_LIFETIME (30)
#define SIZE_X (700)
#define SIZE_Y (350)
#define POS_X (5)
#define POS_Y (70)
#define SCALE (0.37f)
using namespace DirectX;

Coin::Coin() {
	AddComponent(COMPONENT_MODEL);
	//GetModel()->Load("Assets/X_wing/X_wing.fbx");
	GetModel()->Load("Assets/coin.fbx");



	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());


	transform.scale = 50;
	transform.position.x = 100.0f;

	lifeTime = 0;
	colorAlpha = 1.0f;
}


Coin::~Coin() {

}



void Coin::Update() {
	//GetCollider()->mtx = &transform.GetMatrix();
	//GetCollider()->center = (GetModel()->GetCenter());

	if (GetCollider()->Collide()) {
		for (int i = 0; i < GetCollider()->retCollList.size(); i++) {
			if (auto coll = D_CAST(Player, GetCollider()->retCollList[i])) {
				if (!picked) {
					coll->wallet.AddMoney(ADD_NUM);
					picked = true;
				}
			}
		}
	
	}


	transform.rotation.y++;
	
	if (picked) {
		transform.rotation.y += 5;
		transform.position.y += 12;
		GetModel()->SetColor(1.0f, 1.0f, 1.0f, colorAlpha);
		colorAlpha -= 0.1f;
		lifeTime++;
		if (PICKED_LIFETIME < lifeTime) Destroy();
	}

}

void Coin::Draw() {
	ShaderSelector::Get()->SelectModelShader(SHADER_COIN);
	GetModel()->Draw(transform);
}



// À•W0,0,0‚É’u‚­

Wallet::Wallet() {
	money = 9;
	coinTex.Load("Assets/Textures/hp/coin2.png");
	coinTex.scale.x = SIZE_X* SCALE;
	coinTex.scale.y = SIZE_Y* SCALE;
	coinTex.SetAnchor(Sprite::LeftTop);

	coinTex.pos = Vector3(POS_X, POS_Y, 0.0f);

	
}


Wallet::~Wallet() {
}

void Wallet::ScreenDraw() {
	if (GameObjectList::Get()->stopUpdate) return;
	uiWalletNum.Write(Vector3(100, 150), Vector3(50, 50, 1),money);
	coinTex.Render();

}

void Wallet::UseMoney(int num) {
	money -= num;
}

void Wallet::AddMoney(int num) {
	money += num;
}
