#include "Mouse.h"
#include "GameObjectBase.h"

HRESULT Mouse::Initialize(HWND handle, LPDIRECTINPUT8 manager)
{
    //---�e��錾---//
    HRESULT hResult;
    DIPROPDWORD diProperty;

    //---����������---//
    Handle = handle;

    //---�f�o�C�X����---//
    hResult = manager->CreateDevice(GUID_SysMouse, &Device, nullptr);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("�}�E�X�̃I�u�W�F�N�g�����Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //---���͐ݒ�---//
    //�f�[�^�t�H�[�}�b�g�ݒ�
    hResult = Device->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("�}�E�X�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //�������x���̐ݒ�
    hResult = Device->SetCooperativeLevel(Handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("�}�E�X�̋������x���ݒ�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //�f�o�C�X�ݒ�
    diProperty.diph.dwSize = sizeof(DIPROPDWORD);
    diProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diProperty.diph.dwObj = 0;
    diProperty.diph.dwHow = DIPH_DEVICE;
    diProperty.dwData = DIPROPAXISMODE_REL;
    hResult = Device->SetProperty(DIPROP_AXISMODE, &diProperty.diph);
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("�}�E�X�̎��ݒ�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //�f�o�C�X�ւ̓��͐���J�n
    Device->Acquire();
    if (FAILED(hResult))
    {
        MessageBox(Handle, TEXT("�}�E�X�̓��͐���J�n�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return hResult;
    }

    //�}�E�X���W(�X�N���[�����W)�擾
    if (!GetCursorPos(&Position))
    {
        MessageBox(Handle, TEXT("�}�E�X���W�̏������Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return E_FAIL;
    }

    //�E�B���h�E���[�J�����W�ɕϊ�
    if (!ScreenToClient(Handle, &Position))
    {
        MessageBox(Handle, TEXT("�}�E�X���W�̏������Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
        return E_FAIL;
    }

//#ifdef _DEBUG
//    //// �}�E�X������
//    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&*accessINPUTMANAGER->GetManagerDat(), NULL);
//    HRESULT hr = accessINPUTMANAGER->GetManagerDat()->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
//
//    if (hr)
//        return hr;
//
//    // �}�E�X�p�̃f�[�^�t�H�[�}�b�g��ݒ�
//    pDIMouse->SetDataFormat(&c_dfDIMouse2);
//    pDIMouse->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//
//    // �f�o�C�X�̐ݒ�
//    DIPROPDWORD    diprop;
//    diprop.diph.dwSize = sizeof(diprop);
//    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
//    diprop.diph.dwObj = 0;
//    diprop.diph.dwHow = DIPH_DEVICE;
//    diprop.dwData = DIPROPAXISMODE_REL;    // ���Βl���[�h    �i��Βl��REL -> ABS�j
//    pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
//
//    // ���͐���J�n
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
    //---�e��錾---//
    int nCounter;

    //---�f�[�^�̍X�V---//
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
//        // �l������
//        MouseState.rgbButtons[0] = 0;
//        MouseState.rgbButtons[2] = 0;
//        MouseState.lZ = (LONG)0;
//        // �l�X�V
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