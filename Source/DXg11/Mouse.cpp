#include "Mouse.h"
#include "GameObjectBase.h"

HRESULT Mouse::Initialize(HWND handle, LPDIRECTINPUT8 manager)
{
    //---各種宣言---//
    HRESULT hResult;
    DIPROPDWORD diProperty;

    //---初期化処理---//
    Handle = handle;

    //---デバイス生成---//
    hResult = manager->CreateDevice(GUID_SysMouse, &Device, nullptr);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("マウスのオブジェクト生成に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //---入力設定---//
    //データフォーマット設定
    hResult = Device->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("マウスのデータフォーマット設定に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //協調レベルの設定
    hResult = Device->SetCooperativeLevel(Handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("マウスの協調レベル設定に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //デバイス設定
    diProperty.diph.dwSize = sizeof(DIPROPDWORD);
    diProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diProperty.diph.dwObj = 0;
    diProperty.diph.dwHow = DIPH_DEVICE;
    diProperty.dwData = DIPROPAXISMODE_REL;
    hResult = Device->SetProperty(DIPROP_AXISMODE, &diProperty.diph);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("マウスの軸設定に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //デバイスへの入力制御開始
    Device->Acquire();
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("マウスの入力制御開始に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //マウス座標(スクリーン座標)取得
    if (!GetCursorPos(&Position))
    {
        MessageBox(Handle, TEXT("マウス座標の初期化に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return E_FAIL;
    }

    //ウィンドウローカル座標に変換
    if (!ScreenToClient(Handle, &Position))
    {
        MessageBox(Handle, TEXT("マウス座標の初期化に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
        return E_FAIL;
    }

//#ifdef _DEBUG
//    //// マウス初期化
//    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&*accessINPUTMANAGER->GetManagerDat(), NULL);
//    HRESULT hr = accessINPUTMANAGER->GetManagerDat()->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
//
//    if (hr)
//        return hr;
//
//    // マウス用のデータフォーマットを設定
//    pDIMouse->SetDataFormat(&c_dfDIMouse2);
//    pDIMouse->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//
//    // デバイスの設定
//    DIPROPDWORD    diprop;
//    diprop.diph.dwSize = sizeof(diprop);
//    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
//    diprop.diph.dwObj = 0;
//    diprop.diph.dwHow = DIPH_DEVICE;
//    diprop.dwData = DIPROPAXISMODE_REL;    // 相対値モード    （絶対値はREL -> ABS）
//    pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
//
//    // 入力制御開始
//    pDIMouse->Acquire();
//#endif//_DEBUG

    return hResult;
}

void Mouse::Uninitialize(void)
{
    if (Device)
    {
        Device->Unacquire();
    }

    SAFE_RELEASE(Device);
}


void Mouse::Update(void)
{
    //---各種宣言---//
    int nCounter;

    //---データの更新---//
    if (Device)
    {
        Device->Acquire();
        Preview = Current;
        if (Device->GetDeviceState(sizeof(DIMOUSESTATE), &Current) == DIERR_INPUTLOST)
        {
            Device->Acquire();
        }
        else
        {
            GetCursorPos(&Position);
            ScreenToClient(Handle, &Position);
            for (nCounter = 0; nCounter < 4; ++nCounter)
            {
                Trigger.rgbButtons[nCounter] = Current.rgbButtons[nCounter] & ~Preview.rgbButtons[nCounter];
                Release.rgbButtons[nCounter] = ~Current.rgbButtons[nCounter] & Preview.rgbButtons[nCounter];
            }
        }
//#ifdef _DEBUG
//        // 値初期化
//        MouseState.rgbButtons[0] = 0;
//        MouseState.rgbButtons[2] = 0;
//        MouseState.lZ = (LONG)0;
//        // 値更新
//        if (FAILED(pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
//        {
//            pDIMouse->Acquire();
//        }
//#endif//_DEBUG
    }
}

bool Mouse::GetHold(MOUSEBUTTON button)
{
    if (button >= _countof(Current.rgbButtons))
    {
        return false;
    }

    return Current.rgbButtons[button] ? true : false;
}

POINT Mouse::GetCursor(void)
{
    return Position;
}


bool Mouse::GetRelease(MOUSEBUTTON button)
{
    if (button >= _countof(Release.rgbButtons))
    {
        return false;
    }
    return Release.rgbButtons[button] ? true : false;
}

bool Mouse::GetTrigger(MOUSEBUTTON button)
{
    if (button >= _countof(Trigger.rgbButtons))
    {
        return false;
    }
    return Trigger.rgbButtons[button] ? true : false;
}


/////////////////////////////////////////
/////////////////////////////////////////

bool Mouse::GetTriggerClick(int Num)
{
    static bool LastClick[sizeof(MouseState.rgbButtons) / sizeof(MouseState.rgbButtons[0])];

    bool Crnt = GetPressClick(Num);

    bool Ret = (Crnt && !LastClick[Num]);

    LastClick[Num] = Crnt;

    return Ret;
}
bool Mouse::GetTriggerWheel(void)
{
    static bool LastClick = sizeof(MouseState.rgbButtons[2]) / sizeof(MouseState.rgbButtons[2]);

    bool Crnt = GetPressWheel();

    bool Ret = (Crnt && !LastClick);

    LastClick = Crnt;

    return Ret;
}

bool Mouse::GetPressClick(int Num)
{
    return (bool)(MouseState.rgbButtons[Num] & 0x80);
}

bool Mouse::GetPressWheel(void)
{
    return (bool)(MouseState.rgbButtons[2] & 0x80);
}

bool Mouse::GetWheelTriggerUP(void)
{
    if (MouseState.lZ > 0)
        return true;

    return false;
}

bool Mouse::GetWheelTriggerDOWN(void)
{
    if (MouseState.lZ < 0)
        return true;

    return false;
}

DirectX::XMFLOAT2 Mouse::GetMoveVal(void)
{
    return DirectX::XMFLOAT2((float)MouseState.lX, (float)MouseState.lY);
}

float Mouse::GetWheelVal(void)
{
    return (float)MouseState.lZ;
}

//End of File