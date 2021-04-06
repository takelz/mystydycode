
#include"DebugCamera.h"
#include"Camera.h"
#include"Debug/DebugText.h"
#include"Game.h"

//#define		DEBUG_CAMERA_ON 

#define		PI					(3.1415926f)      
#define		DEBUG_CAMERA_SPD	(5.0f)
#define		DEBUG_CAMERA_ROTSPED (1.0f)
#define		SPEED_AXIS_Z (1.4f)
#define		SPEED_AXIS_XY (1.5f)
#define		SPEED_ROTATE (0.2f)
#define		INPUT_KEY_FORWARD (DIK_A)
#define		INPUT_KEY_BACK (DIK_S)
#define		LENGHT_LOOK (500.0f)

Vector3 DebugCamera::paramPosition;
Vector3 DebugCamera::paramRotation;
Vector3 DebugCamera::paramLookAt;
bool DebugCamera::debugOn;
bool DebugCamera::lookMode;

using namespace DirectX;


Vector3 MousePosition() {
	POINT lpPoint;
	Vector3 mousePos;
	RECT* rect = new RECT();

	GetWindowRect(GetHwnd(),rect);
	GetCursorPos(&lpPoint);
	ScreenToClient(GetHwnd(), &lpPoint);
	

	mousePos.x = (float)lpPoint.x;
	mousePos.y = (float)lpPoint.y;
	mousePos.z = 0.0f;

	static bool trg = false;
	if (Input::GetKey(DIK_M, INPUT::TRIGGER)) {
		trg = !trg;
	}
	if(!trg)
		SetCursorPos(SCREEN_X / 2 + rect->left, SCREEN_Y / 2 + rect->top);


	// 操作不能を防ぐために常に最前面へ
	SetForegroundWindow(GetHwnd());

	return mousePos;
}

DebugCamera::DebugCamera() {
	DontDestroy();
}


void DebugCamera::Update() {

	Move();

	if (Input::GetKey(DIK_9, Input::TRIGGER)) {
#ifdef DEBUG_CAMERA_ON
		debugOn = !debugOn;
		MessageBox(NULL, std::to_string(debugOn).c_str(), "カメラモードを変えました", MB_OK);
#endif
	}
	if (debugOn) {
		controlCamera.ClickMove();
		controlCamera.Move();
		controlCamera.ClickRotate();
		controlCamera.Rotate();	
	}
	
	//SetData();
}


void DebugCamera::Draw() {
}

void DebugCamera::SetData() {
	Camera::Instance()->paramPosition = paramPosition;
	Camera::Instance()->paramRotation = paramRotation;

}

void DebugCamera::Move() {

	// 移動
	if (Input::GetKey(DIK_W, Input::HOLD)) {
		paramPosition.z+= DEBUG_CAMERA_SPD;
	}
	if (Input::GetKey(DIK_S, Input::HOLD)) {
		paramPosition.z-= DEBUG_CAMERA_SPD;
	}
	if (Input::GetKey(DIK_D, Input::HOLD)) {
		paramPosition.x+= DEBUG_CAMERA_SPD;
	}
	if (Input::GetKey(DIK_A, Input::HOLD)) {
		paramPosition.x-= DEBUG_CAMERA_SPD;
	}
	if (Input::GetKey(DIK_Q, Input::HOLD)) {
		paramPosition.y+= DEBUG_CAMERA_SPD;
	}
	if (Input::GetKey(DIK_E, Input::HOLD)) {
		paramPosition.y-= DEBUG_CAMERA_SPD;
	}
	// 回転
	if (Input::GetKey(DIK_RIGHTARROW,Input::HOLD)) {
		paramRotation.y += DEBUG_CAMERA_ROTSPED;
	}
	if (Input::GetKey(DIK_LEFTARROW, Input::HOLD)) {
		paramRotation.y -= DEBUG_CAMERA_ROTSPED;
	}
	if (Input::GetKey(DIK_UPARROW, Input::HOLD)) {
		paramRotation.x += DEBUG_CAMERA_ROTSPED;
	}
	if (Input::GetKey(DIK_DOWNARROW, Input::HOLD)) {
		paramRotation.x -= DEBUG_CAMERA_ROTSPED;
	}


}


void EditorControlCamera::Move() {
	if (Input::GetKey(INPUT_KEY_BACK, Input::HOLD)) {
		Vector3 dir = Camera::Instance()->GetLookVector();
		Camera::Instance()->paramPosition.z += SPEED_AXIS_Z * dir.z;
		Camera::Instance()->paramPosition.x += SPEED_AXIS_Z * dir.x;
		Camera::Instance()->paramPosition.y += SPEED_AXIS_Z * dir.y;

	}
	if (Input::GetKey(INPUT_KEY_FORWARD, Input::HOLD)) {
		Vector3 dir = Camera::Instance()->GetLookVector();
		Camera::Instance()->paramPosition.z -= SPEED_AXIS_Z * dir.z;
		Camera::Instance()->paramPosition.x -= SPEED_AXIS_Z * dir.x;
		Camera::Instance()->paramPosition.y -= SPEED_AXIS_Z * dir.y;
	}

	//
	if (!Input::GetMouse(MOUSEBUTTON_CENTER, Input::HOLD)) return;	// 押し込んでないならスルー
	
	Vector3 calcNum;
	Vector3 dir = Camera::Instance()->GetLookVector();

	calcNum.x = (MousePosition().x - stackClickMovePos.x);
	calcNum.z = (MousePosition().x - stackClickMovePos.x);

	calcNum.y = MousePosition().y - stackClickMovePos.y;


	Camera::Instance()->paramPosition.x += calcNum.x * SPEED_AXIS_XY * dir.z;
	Camera::Instance()->paramPosition.z -= calcNum.z * SPEED_AXIS_XY * dir.x;

	Camera::Instance()->paramPosition.y += calcNum.y * SPEED_AXIS_XY;

	stackClickMovePos.x = MousePosition().x;
	stackClickMovePos.y = MousePosition().y;
	

}

void EditorControlCamera::ClickMove() {
	if (Input::GetMouse(MOUSEBUTTON_CENTER, Input::TRIGGER)) {
		stackClickMovePos.x = GetMousePos().x;
		stackClickMovePos.y = GetMousePos().y;
	}
}

void EditorControlCamera::Rotate() {
	if (!Input::GetMouse(MOUSEBUTTON_RIGHT, Input::HOLD)) return;	// 押し込んでないならスルー
	Vector3 calcNum;

	calcNum.x = GetMousePos().x - stackClickRotationPos.x;
	calcNum.y = GetMousePos().y - stackClickRotationPos.y;

	Camera::Instance()->paramRotation.x -= calcNum.y * SPEED_ROTATE;
	Camera::Instance()->paramRotation.y += calcNum.x * SPEED_ROTATE;

	stackClickRotationPos.x = GetMousePos().x;
	stackClickRotationPos.y = GetMousePos().y;

}


void EditorControlCamera::ClickRotate() {
	if (Input::GetMouse(MOUSEBUTTON_RIGHT, Input::TRIGGER)) {
		stackClickRotationPos.x = GetMousePos().x;
		stackClickRotationPos.y = GetMousePos().y;
	}
}



