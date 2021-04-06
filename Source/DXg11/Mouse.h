#ifndef _MOUSE_H_
#define _MOUSE_H_
#define    DIRECTINPUT_VERSION    0x0800

//＝＝＝ヘッダファイル読み込み＝＝＝//
#include <dinput.h>    // DirectInputバージョンの宣言
#include <DirectXMath.h>
#include <array>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//＝＝＝列挙型定義＝＝＝//
typedef enum
{
    MOUSEBUTTON_LEFT,
    MOUSEBUTTON_RIGHT,
    MOUSEBUTTON_CENTER,
    MOUSEBUTTON_MAX
}MOUSEBUTTON;

//＝＝＝クラス宣言＝＝＝//
class Mouse
{
    private:
        LPDIRECTINPUTDEVICE8 Device; //デバイス
        POINT Position;              //カーソル座標
        DIMOUSESTATE Current;         //現在の状態
        DIMOUSESTATE Preview;         //前回の状態
        DIMOUSESTATE Trigger;         //トリガ状態
        DIMOUSESTATE Release;         //リリース状態
        HWND Handle;                 //ウィンドウハンドル
#ifdef _DEBUG
        DIMOUSESTATE2            MouseState;            // マウス状態
        LPDIRECTINPUTDEVICE8    pDIMouse;    // マウスデバイス

#endif
    public:
        HRESULT Initialize(HWND, LPDIRECTINPUT8);
        void Uninitialize(void);
        void Update(void);

		// 押し続けているか
        bool GetHold(MOUSEBUTTON);
		// カーソルの位置
        POINT GetCursor(void);
		// 離したとき
        bool GetRelease(MOUSEBUTTON);
		// 押した瞬間
        bool GetTrigger(MOUSEBUTTON);

        bool  GetWheelTriggerUP(void);
        bool  GetWheelTriggerDOWN(void);
        bool  GetPressWheel(void);
        bool  GetTriggerWheel(void);
        float  GetWheelVal(void);
        DirectX::XMFLOAT2 GetMoveVal(void);
        bool GetPressClick(int Num);
        bool GetTriggerClick(int Num);

};

#endif