 
#include"TitleCenter.h"
#include<Camera.h>

#define OFFSET_Y (50.0f)
#define CAMERA_HEIGHT (450)
#define CAMERA_DISTANCE (850)
#define CAMERA_ROTSPD (0.5f)

TitleCenter::TitleCenter() {

	Camera::Instance()->paramPosition = Vector3(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE);
	Camera::Instance()->paramRotation.x = 5.0f;

	// タイトルとスタート表示
	GameObjectList::Get()->Generate(new TitleName());
	GameObjectList::Get()->Generate(new StartGame());


}

void TitleCenter::Update() {
	CalcRotateCamera();
}

void TitleCenter::Draw() {

}

void TitleCenter::CalcRotateCamera() {
	Camera::Instance()->notUpdate = true;

	static float angle;
	static XMFLOAT3 UP = XMFLOAT3(0, 1, 0);
	XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(angle));
	XMFLOAT3 f3Pos = XMFLOAT3(0.0f, 5.0f, 0.0f);
	XMFLOAT3 look = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMMATRIX mMove = XMMatrixTranslation(f3Pos.x, f3Pos.y, f3Pos.z);
	XMVECTOR vPos = XMVector3TransformCoord(XMVectorSet(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE, 1.0f),
		XMMatrixMultiply(mRotate, mMove));
	XMStoreFloat3(&f3Pos, vPos);
	angle += CAMERA_ROTSPD;
	if (angle >= 180.0f)
		angle -= 360.0f;
	Camera::Instance()->SetEye(f3Pos);
	Camera::Instance()->SetLookat(look);
	Camera::Instance()->SetUp(UP);
	Camera::Instance()->CreateCameraMatrix();
;

}

#define TITLE_WIDTH (1000)
#define TITLE_HIEGHT (200)
#define TITLE_POS_Y (100)


TitleName::TitleName() {
	AddComponent(COMPONENT_SPRITE);
	GetSprite()->Load("Assets/textures/title/titlename.png");
	GetSprite()->scale = Vector3(TITLE_WIDTH, TITLE_HIEGHT, 0.0f);
	GetSprite()->SetAnchor(Sprite::Center);
	GetSprite()->pos.x = SCREEN_X/2;
	GetSprite()->pos.y = TITLE_POS_Y;

	start.Load("Assets/textures/title/titlesstart.png");
	start.scale = Vector3(220, 110);
	start.pos.x = SCREEN_X / 2;
	start.pos.y = TITLE_POS_Y + 470;
	start.SetAnchor(Sprite::Center);
}

void TitleName::Update() {

}

void TitleName::ScreenDraw() {
	GetSprite()->Render();
	start.Render();
}