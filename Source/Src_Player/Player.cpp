#include"Player.h"
#include"../Camera.h"

#include"../DXg11/Mathf.h"
#include"../StandardBullet.h"
#include"../DirectX/DrawObject.h"
#include"../Src_Enemy/EnemyBase.h"
#include"../DXg11/SceneManager.h"
#include"../StageManager.h"

#define ROTATE_SPEED (0.5f)
#define ROTATE_RATE_Y_SPEED (0.5f)	// Y軸のカメラ移動でROTATE_SPEEDよりどれぐらい減速させるか
#define DASH_SPD (1.5f)
#define DEFAULT_LIFE (7)

#define CAMERA_BACK (420)
#define CAMERA_UP (95)

#define MAX_MOVE_DISTANCE_CAMERA (20) // 全身したときのカメラ離れる距離
#define MAX_MOVE_ACC_CAMERA (0.5f) // 加速


#define UI_SIZE_X (800* 0.7f)
#define UI_SIZE_Y (100* 0.7f)
#define UI_POS_X (SCREEN_X/2)
#define UI_POS_Y (80)

#define SPD_DAMAGE_COLOR (0.01f)
#define OFFSET_DAMAGE_NUM (0.30f);	// 何パーセント削りからダメージ表現をするか

bool Player::atkPowerUp = false;

static bool loaded = false;
static RenderModel md[5];
Player::Player() {
	// 緊急でモデル一括読み込みをここで


	md[0].Load("Assets/Player/fighter01.fbx");
	md[1].Load("Assets/UNIT2/baseturret.fbx");
	md[2].Load("Assets/crystals/crystals.fbx");
	md[3].Load("Assets/coin.fbx");
	md[4].Load("Assets/Enemy/shot/frigate01.fbx");


	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/Player/fighter01.fbx");
	transform.position.z = 1000;
	transform.position.x = 200;
	transform.position.y = 100;
	transform.scale *= 25.0f;
	 
	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());


	camObj = new DebugCamera();
	GameObjectList::Get()->Generate(camObj);



	SetTag(TAG_PLAYER);
	inputMove.trans = &transform;

	life = DEFAULT_LIFE;
	dash = 0.0f;
	atkPowerUp = false;	// もし複数作るならシーンのオブジェクトに書く
	lifeUI.SetMaxLife(DEFAULT_LIFE, Vector3(UI_POS_X, UI_POS_Y));
	lifeUI.SetScale(Vector3(UI_SIZE_X, UI_SIZE_Y));

	// ランダムテクスチャ(ダメージ表現用)
	randomGraphicTexture.Load("Assets/Textures/shader/damage.png");
	pRandomTexture = randomGraphicTexture.pTexture;

	// バッファ作成
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(Buffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &constBuffer);
}
void Player::Start() {
	// 描画順の関係
	//barrier = new Barrier();
	//GameObjectList::Get()->Generate(barrier);
}

DebugCamera *ccc = nullptr;
void Player::Update() {
	if (camObj->debugOn) return;	// デバッグモードオンならプレイヤー無効
	if (damageColor > 0) {
		damageColor -= SPD_DAMAGE_COLOR;
	}
	Input();

	Rotate();
	FollowCamera();
	DispUI();
	BarrierSystem();

	ccc = camObj;
}

DebugCamera* CCC() {
	return ccc;
}

void Player::Draw() {
	setBuffer.time.x = 1.0f;
	setBuffer.time.y = ((float)life / (float)DEFAULT_LIFE) + OFFSET_DAMAGE_NUM;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(2, 1, &constBuffer);
	ShaderSelector::Get()->SetConstantBuffers()->PSSetShaderResources(2, 1, &pRandomTexture);
	GetModel()->SetColor(1.0f, 1.0f - damageColor / 2, 1.0f - damageColor);
	ShaderSelector::Get()->SelectModelShader(SHADER_DAMAGE_EMISSION);
	
	GetModel()->SetTextureWrap(true);
	GetModel()->Draw(transform);
	
	if (!loaded) {
		for (int i = 0; i < 5; i++) md[i].Draw(transform);
		loaded = true;
	}
}

void Player::LateDraw(){

}


void Player::ScreenDraw() {
	wallet.ScreenDraw();
}



void Player::Input() {

	inputMove.Update();

	if (Input::GetKey(DIK_SPACE, Input::TRIGGER)|| DIGetPadTrigger(PAD_R1)) {
		StandardBullet *obj = new StandardBullet();
		obj->transform.position = transform.position;
		obj->transform.rotation = transform.rotation;
		obj->direction = transform.Forward();
		GameObjectList::Get()->Generate(obj);
		if (atkPowerUp) {

		}
	}
	if (Input::GetKey(DIK_E, Input::TRIGGER) || DIGetPadTrigger(PAD_CROSS)) {
		if (wallet.money <= 0) return;
		wallet.UseMoney(3);
		UnitNormal *obj = new UnitNormal();
		obj->transform.position = transform.position;
		obj->transform.rotation.y = transform.rotation.y;
		obj->transform.position.x += transform.Forward().x * 100;

		GameObjectList::Get()->Generate(obj);
	}

	// ストア

	if (Input::GetKey(DIK_R, Input::TRIGGER) || DIGetPadTrigger(PAD_SQUARE)) {
		//marketCenter->BuyPlayerPower(&wallet.money);
		marketCenter->BuyBarrier(&wallet.money);
	}
	if (Input::GetKey(DIK_Q, Input::TRIGGER) || DIGetPadTrigger(PAD_CIRCLE)) {
		marketCenter->BuyPlayerLife(&wallet.money,&life);
	}
	if (Input::GetKey(DIK_3, Input::TRIGGER) || DIGetPadTrigger(PAD_TRIANGLE)) {
		//marketCenter->BuyTurretPower(&wallet.money);
	}
	if (Input::GetKey(DIK_P, Input::TRIGGER)) {
		//marketCenter->OpenClose();
	}

}

void Player::Rotate() {
	static float posY, camY;

	transform.rotation.y -= (posY - MousePosition().x) * ROTATE_SPEED;
	posY = MousePosition().x;

	transform.rotation.y += DIGetPadStick(PAD_ST_R_X) * ROTATE_SPEED * 5;

	// カメラの回転
	//Camera::Instance()->paramRotation.x -= (camY - MousePosition().y) * ROTATE_SPEED * ROTATE_RATE_Y_SPEED;
	//camY = MousePosition().y;

}

void Player::FollowCamera() {
	static float dist;
	if (Input::GetKey(DIK_W, Input::HOLD) || (DIGetPadStick(PAD_ST_L_Y) < -0.2f)) {
		if (dist < MAX_MOVE_DISTANCE_CAMERA) dist += MAX_MOVE_ACC_CAMERA;
	}
	else {
		if (dist > 0.0f) dist -= MAX_MOVE_ACC_CAMERA * 2;
		else dist = 0.0f;
	}
	// カメラ
	Camera::Instance()->paramPosition.x = transform.position.x - Mathf::AngleYToVector(transform.rotation.y).x * (CAMERA_BACK + dist);
	Camera::Instance()->paramPosition.y = transform.position.y + CAMERA_UP;
	Camera::Instance()->paramPosition.z = transform.position.z - Mathf::AngleYToVector(transform.rotation.y).z * (CAMERA_BACK + dist);
	Camera::Instance()->paramRotation.y = transform.rotation.y;

}

void Player::DispUI() {


	lifeUI.SetCurrentLife(life);
}

void Player::Recovery() {

}

void Player::BarrierSystem(){
	//barrier->transform.position = transform.position;
	
}

void Player::AddDamage() {
	life--;
	damageColor = 1.0f;
	if (life <= 0) {
		StageManager::Get()->GameOver();
	}
}

