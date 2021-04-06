#include"Input.h"

#define     SAFE_RELEASE(p)         if(p){ p->Release(); p = nullptr; }

//�������O���[�o���錾������//
LPDIRECTINPUT8 Input::Manager;
Keyboard Input::keyboard;
Mouse Input::mouse;

#define MAX_JOY		4		// �ő�ڑ���
#define JOY_MIN_X	-32768	// �w���ŏ��l
#define JOY_MAX_X	32767	// �w���ő�l
#define JOY_MIN_Y	-32768	// �x���ŏ��l
#define JOY_MAX_Y	32767	// �x���ő�l
#define CALC_TO_FLOAT_MAX	(-32768)
#define CALC_TO_FLOAT_MIN	(32767)

#define HALF (16384)

#define RANGE_ZERO_STICK (2500)	// 0~�w�萔�l�͈̔͂Ȃ���͖���

static LPDIRECTINPUTDEVICE8	g_pDIDeviceJoy[MAX_JOY];		// �Q�[���p�b�h �I�u�W�F�N�g

static DWORD				g_dwJoystick;					// �Q�[���p�b�h�ڑ���
static DIJOYSTATE			g_diJoyState[MAX_JOY];			// �Q�[���p�b�h���
static DIJOYSTATE			g_diPrevJoyState[MAX_JOY];		// �O��̃Q�[���p�b�h���
static DIJOYSTATE			g_diJoyTriggerState[MAX_JOY];	// �Q�[���p�b�h �g���K���
static DIJOYSTATE			g_diJoyReleaseState[MAX_JOY];	// �Q�[���p�b�h �����[�X���

static bool					g_joyOn;						// �Q�[���p�b�g���ǂݍ��܂ꂽ��g�p

#pragma region	// input keyboard

HRESULT Keyboard::Initialize(HWND handle, LPDIRECTINPUT8 manager)
{
	//---�e��錾---//
	HRESULT hResult;

	//---�f�o�C�X����---//
	hResult = manager->CreateDevice(GUID_SysKeyboard, &Device, nullptr);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("�L�[�{�[�h�̃I�u�W�F�N�g�����Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
		return hResult;
	}

	//---���͐ݒ�---//
	//�f�[�^�t�H�[�}�b�g�ݒ�
	hResult = Device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
		return hResult;
	}

	//�������x���̐ݒ�
	hResult = Device->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("�L�[�{�[�h�̋������x���ݒ�Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
		return hResult;
	}


	//�f�o�C�X�ւ̓��͐���J�n
	Device->Acquire();

	return hResult;
}


void Keyboard::Uninitialize()
{
	if (Device){
		Device->Unacquire();
	}

	SAFE_RELEASE(Device);
}


void Keyboard::Update() {
	//---�e��錾---//
	int nCounter;

	//---�f�[�^�̍X�V---//
	if (Device){
		Device->Acquire();
		Preview = Current;
		if (Device->GetDeviceState(256, (void*)&Current) == DIERR_INPUTLOST)
		{
			Device->Acquire();
		}
		else
		{
			for (nCounter = 0; nCounter < 256; ++nCounter)
			{
				Trigger.at(nCounter) = Current.at(nCounter) & ~Preview.at(nCounter);
				Release.at(nCounter) = ~Current.at(nCounter) & Preview.at(nCounter);
			}
		}
	}
}


bool Keyboard::GetHold(DWORD button){
	return Current.at(button & 0xFF) ? true : false;
}


bool Keyboard::GetRelease(DWORD button){
	return Release.at(button & 0xFF) ? true : false;
}


bool Keyboard::GetTrigger(DWORD button){
	return Trigger.at(button & 0xFF) ? true : false;
}


#pragma endregion

// �Q�[���p�b�h�₢���킹�p�R�[���o�b�N�֐�
static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS diDevCaps;
	HRESULT   hr;

	hr = Input::GetManagerDat()->CreateDevice(lpddi->guidInstance, &g_pDIDeviceJoy[g_dwJoystick], NULL);
	if (FAILED(hr)) {
		return DIENUM_CONTINUE;
	}
	diDevCaps.dwSize = sizeof(diDevCaps);
	hr = g_pDIDeviceJoy[g_dwJoystick]->GetCapabilities(&diDevCaps);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceJoy[g_dwJoystick]);
		return DIENUM_CONTINUE;
	}
	if (++g_dwJoystick < MAX_JOY) return DIENUM_CONTINUE;
	return DIENUM_STOP;
}



HRESULT Input::Initialize(HINSTANCE instance, HWND handle)
{
	//---�e��錾---//
	HRESULT hResult;

	//DirectInput�I�u�W�F�N�g����
	hResult = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&Manager, nullptr);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("DirectInput�I�u�W�F�N�g�����Ɏ��s���܂���"), TEXT("�������G���["), MB_ICONSTOP | MB_OK);
		Uninitialize();
		return hResult;
	}


	//�L�[�{�[�h�̏�����
	hResult = keyboard.Initialize(handle, Manager);
	if (FAILED(hResult)){
		Uninitialize();
		return hResult;
	}

	//�}�E�X�̏�����
	hResult = mouse.Initialize(handle, Manager);
	if (FAILED(hResult))
	{
		Uninitialize();
		return hResult;
	}

	// �Q�[���p�b�h������
	//////////////////////////////////////////////////////////////////////////////////////

	g_dwJoystick = 0;
	hResult = Input::GetManagerDat()->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hResult)) {
		MessageBoxA(NULL, "�Q�[���p�b�h �I�u�W�F�N�g�����Ɏ��s���܂����B", "error", MB_ICONSTOP | MB_OK);
		return hResult;
	}
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		// �f�[�^ �t�H�[�}�b�g�ݒ�
		hResult = g_pDIDeviceJoy[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hResult)) {
			MessageBoxA(NULL, "�Q�[���p�b�h �f�[�^ �t�H�[�}�b�g�ݒ�Ɏ��s���܂����B", "error", MB_ICONSTOP | MB_OK);
			return hResult;
		}
		// �������x���̐ݒ�
		hResult = g_pDIDeviceJoy[i]->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hResult)) {
		
			MessageBoxA(NULL, "�Q�[���p�b�h�������x���ݒ�Ɏ��s���܂����B", "error", MB_ICONSTOP | MB_OK);
			return hResult;
		}
		// �f�o�C�X�ݒ�
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_X;
		diprg.lMax = JOY_MAX_X;
		g_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_Y;
		diprg.lMax = JOY_MAX_Y;
		g_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		g_joyOn = true;
	}
	
//////////////////////////////////////////////////////////////////////////////////////


	return hResult;
}


void Input::Uninitialize(){

	keyboard.Uninitialize();
	mouse.Uninitialize();
	SAFE_RELEASE(Manager);
}


void Input::Update(){
	keyboard.Update();
	mouse.Update();
	
	HRESULT hResult;
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		g_pDIDeviceJoy[i]->Acquire();
		g_pDIDeviceJoy[i]->Poll();
		CopyMemory(&g_diPrevJoyState[i], &g_diJoyState[i], sizeof(g_diJoyState[0]));
		hResult = g_pDIDeviceJoy[i]->GetDeviceState(sizeof(g_diJoyState[0]), &g_diJoyState[i]);
		if (hResult == DIERR_INPUTLOST)
			g_pDIDeviceJoy[i]->Acquire();
		else {
			for (int j = 0; j < 32; ++j) {
				g_diJoyTriggerState[i].rgbButtons[j] = (~g_diPrevJoyState[i].rgbButtons[j] & g_diJoyState[i].rgbButtons[j]) & 0x80;
				g_diJoyReleaseState[i].rgbButtons[j] = (g_diPrevJoyState[i].rgbButtons[j] & ~g_diJoyState[i].rgbButtons[j]) & 0x80;
			}
		}
	}
}



bool Input::GetKey(DWORD button, PUSHTYPE type){
	switch (type){
		case HOLD:
			return keyboard.GetHold(button);

		case TRIGGER:
			return keyboard.GetTrigger(button);

		case RELEASE:
			return keyboard.GetRelease(button);

		default:
			return false;
	}
}

bool Input::GetMouse(MOUSEBUTTON button, PUSHTYPE type) {
	switch (type) {
	case HOLD:
		return mouse.GetHold(button);

	case TRIGGER:
		return mouse.GetTrigger(button);

	case RELEASE:
		return mouse.GetRelease(button);

	default:
		return false;
	}
}

POINT Input::GetMousePosition() {
	return mouse.GetCursor();
	

}



// ��������Q�[���p�b�h

// �Q�[���p�b�h���擾
DWORD DIGetJoyCount()
{
	return g_dwJoystick;
}

// �Q�[���p�b�h��Ԏ擾
DIJOYSTATE *DIGetJoy(DWORD dwJoy)
{

	if (!g_joyOn) {
		DIJOYSTATE joy;
		return &joy;
	}
	if (dwJoy >= g_dwJoystick) return NULL;
	return &g_diJoyState[dwJoy];
}

// �Q�[���p�b�hX���擾
LONG DIGetJoyX(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lX;
}

// �Q�[���p�b�hY���擾
LONG DIGetJoyY(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lY;
}

// �Q�[���p�b�hZ���擾
LONG DIGetJoyZ(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lZ;
}

// �Q�[���p�b�h �{�^��������Ԏ擾
bool DIGetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (!g_joyOn) return false;
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyState[dwJoy].rgbButtons)) return false;
	return (g_diJoyState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// �Q�[���p�b�h �{�^�� �g���K��Ԏ擾
bool DIGetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (!g_joyOn) return false;
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyTriggerState[dwJoy].rgbButtons)) return false;
	return (g_diJoyTriggerState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// �Q�[���p�b�h �{�^�� �����[�X��Ԏ擾
bool DIGetJoyRelease(DWORD dwJoy, DWORD dwBtn)
{
	if (!g_joyOn) return false;
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyReleaseState[dwJoy].rgbButtons)) return false;
	return (g_diJoyReleaseState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}


bool DIGetPadTrigger(DWORD dwBtn) {
	return DIGetJoyTrigger(0, dwBtn);
}


bool DIGetPadButton(DWORD dwBtn) {
	return DIGetJoyButton(0, dwBtn);
}


bool DIGetPadRelease(DWORD dwBtn) {
	return DIGetJoyRelease(0, dwBtn);
}

float DIGetPadStick(DWORD dwStk) {
	return DIGetPadStick(dwStk, 0);
}

float DIGetPadStick(DWORD dwStk, DWORD adj) {
	if (!g_joyOn) return 0.0f;			// �g���ĂȂ��Ȃ炱���ŏI��

	static const float adjNum = 1024;	// �v�Z�p�ɒl���������������ʂ�MAX�l
	LONG retNum = 0;
	switch (dwStk) {
	case PAD_ST_L_X: retNum = DIGetJoy(0)->lX;
		break;
	case PAD_ST_L_Y: retNum = DIGetJoy(0)->lY;
		break;
	case PAD_ST_R_X: retNum = DIGetJoy(0)->lZ;
		retNum += CALC_TO_FLOAT_MAX;	// ������0~�Ȃ̂�
		break;
	case PAD_ST_R_Y: retNum = DIGetJoy(0)->lRz;
		retNum += CALC_TO_FLOAT_MAX;	// ������0~�Ȃ̂�
		break;
	case PAD_L2: retNum = DIGetJoy(0)->lRx;
		break;
	case PAD_R2: retNum = DIGetJoy(0)->lRy;
		break;
	default:
#ifdef _DEBUG
		MessageBox(NULL, "DigetPadStick�̈������Ԉ���Ă��܂�", "�p�b�h�G���[", MB_OK);
#endif // _DEBUG
		break;
	}
	if (abs(retNum) < RANGE_ZERO_STICK) {	// �l������������Ȃ�f�t�H���g�l��Ԃ�
		if (!adj) return 0;		// �l��0����Ȃ�
		if (adj) return 1.0f;		// �l���}�C�i�X����Ȃ�
	}
	float calcOut = 0.0f;	// �v�Z�p�ϐ�
	retNum = retNum >> 5;	// 1024�ɒl�����������Ă���v�Z	


	// ���̒l�̍ő�l��1����Ȃ��Ȃ邽�ߊ����̍ő�l��0.99�ɂȂ��Ă��܂��̂�adjNum-1�̒l�Ȃ�MAX�Ƃ��Ĉ���
	if (retNum == adjNum - 1) retNum = (LONG)adjNum;

	calcOut = (float)retNum / 1024;	//0.0f~1.0f��(���x�͖���)


	if (adj) {	// �w�肳�ꂽ�Ȃ�}�C�i�X����ł͂Ȃ��[������̒l��
		calcOut += 1.0f;
	}

	return calcOut;
}