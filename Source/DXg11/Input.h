#pragma once


#define    DIRECTINPUT_VERSION    0x0800
#include <dinput.h>    // DirectInputバージョンの宣言
#include <DirectXMath.h>
#include <array>
#include "Mouse.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



#define PUSHTYPE        Input::PUSH_TYPE
#define INPUT Input


enum {
	PAD_SQUARE,		// □
	PAD_CROSS,		// ×
	PAD_CIRCLE,		// ○
	PAD_TRIANGLE,	// △
	PAD_L1,
	PAD_R1,
	PAD_L2,		// ボタンとスティック両方可能
	PAD_R2,		// ボタンとスティック両方可能
	PAD_SHARE,
	PAD_START,
	PAD_L3,
	PAD_R3,
	PAD_HOME,	//　使わないこと
	PAD_TOUCH,
	PAD_ST_L_X,
	PAD_ST_L_Y,
	PAD_ST_R_X,
	PAD_ST_R_Y,
	PAD_UP,		// 十字キー上　未実装
	PAD_DOWN,	// 十字キー下  未実装
	PAD_RIGHT,	// 十字キー右  未実装
	PAD_LEFT,	// 十字キー左  未実装
};



//＝＝＝クラス宣言＝＝＝//
class Keyboard
{
private:
	LPDIRECTINPUTDEVICE8 Device;    //デバイス
	std::array<BYTE, 256> Current;    //現在の状態
	std::array<BYTE, 256> Preview;    //前回の状態
	std::array<BYTE, 256> Trigger;    //トリガ状態
	std::array<BYTE, 256> Release;    //リリース状態

public:
	HRESULT Initialize(HWND, LPDIRECTINPUT8);
	void Uninitialize(void);
	void Update(void);

	bool GetHold(DWORD);
	bool GetRelease(DWORD);
	bool GetTrigger(DWORD);
};

//＝＝＝クラス宣言＝＝＝//
class Input
{
public:
	typedef enum
	{
		HOLD,
		RELEASE,
		TRIGGER,
		PUSHTYPE_MAX
	}PUSH_TYPE;
private:
	static LPDIRECTINPUT8 Manager;
	static Keyboard keyboard;
	static Mouse	mouse;

public:
	static HRESULT Initialize(HINSTANCE, HWND);
	static void Uninitialize(void);
	static void Update(void);

	static bool GetKey(DWORD, PUSHTYPE);
	static bool GetMouse(MOUSEBUTTON, PUSHTYPE);
	static POINT GetMousePosition(void);

public:

	static Keyboard* GetKeyboardDat(void) {
		return &keyboard;
	}
	static LPDIRECTINPUT8 GetManagerDat(void) {
		return Manager;
	}
	static Mouse* GetMouseDat() {
		return &mouse;
	}
};


DWORD DIGetJoyCount();
DIJOYSTATE *DIGetJoy(DWORD dwJoy);
LONG DIGetJoyX(DWORD dwJoy);
LONG DIGetJoyY(DWORD dwJoy);
LONG DIGetJoyZ(DWORD dwJoy);
bool DIGetJoyButton(DWORD dwJoy, DWORD dwBtn);
bool DIGetJoyTrigger(DWORD dwJoy, DWORD dwBtn);
bool DIGetJoyRelease(DWORD dwJoy, DWORD dwBtn);
// ボタンの指定は列挙体の　PAD_ ~~
bool DIGetPadTrigger(DWORD dwBtn);
// ボタンの指定は列挙体の　PAD_ ~~
bool DIGetPadButton(DWORD dwBtn);
// ボタンの指定は列挙体の　PAD_ ~~
bool DIGetPadRelease(DWORD dwBtn);


// パッドのスティックの値を返す。値は-1.0f~1.0f
// PAD_NUM_ADJ の定数を二番目の引数に入れると0~2.0fの値が返ってくる。
// ボタンの指定は列挙体の PAD_~~ で指定する。
// R2とL2も対応。 
float DIGetPadStick(DWORD dwStk, DWORD adj);

// パッドのスティックの値を返す。値は-1.0f~1.0f(引数が一つのみの場合)
// ボタンの指定は列挙体の PAD_~~ で指定する。
// R2とL2も対応。 
float DIGetPadStick(DWORD dwStk);