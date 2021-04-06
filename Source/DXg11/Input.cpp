#include"Input.h"

#define     SAFE_RELEASE(p)         if(p){ p->Release(); p = nullptr; }

//＝＝＝グローバル宣言＝＝＝//
LPDIRECTINPUT8 Input::Manager;
Keyboard Input::keyboard;
Mouse Input::mouse;

#define MAX_JOY		4		// 最大接続数
#define JOY_MIN_X	-32768	// Ｘ軸最小値
#define JOY_MAX_X	32767	// Ｘ軸最大値
#define JOY_MIN_Y	-32768	// Ｙ軸最小値
#define JOY_MAX_Y	32767	// Ｙ軸最大値
#define CALC_TO_FLOAT_MAX	(-32768)
#define CALC_TO_FLOAT_MIN	(32767)

#define HALF (16384)

#define RANGE_ZERO_STICK (2500)	// 0~指定数値の範囲なら入力無効

static LPDIRECTINPUTDEVICE8	g_pDIDeviceJoy[MAX_JOY];		// ゲームパッド オブジェクト

static DWORD				g_dwJoystick;					// ゲームパッド接続数
static DIJOYSTATE			g_diJoyState[MAX_JOY];			// ゲームパッド状態
static DIJOYSTATE			g_diPrevJoyState[MAX_JOY];		// 前回のゲームパッド状態
static DIJOYSTATE			g_diJoyTriggerState[MAX_JOY];	// ゲームパッド トリガ状態
static DIJOYSTATE			g_diJoyReleaseState[MAX_JOY];	// ゲームパッド リリース状態

static bool					g_joyOn;						// ゲームパットが読み込まれたら使用

#pragma region	// input keyboard

HRESULT Keyboard::Initialize(HWND handle, LPDIRECTINPUT8 manager)
{
	//---各種宣言---//
	HRESULT hResult;

	//---デバイス生成---//
	hResult = manager->CreateDevice(GUID_SysKeyboard, &Device, nullptr);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("キーボードのオブジェクト生成に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
		return hResult;
	}

	//---入力設定---//
	//データフォーマット設定
	hResult = Device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("キーボードのデータフォーマット設定に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
		return hResult;
	}

	//協調レベルの設定
	hResult = Device->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("キーボードの協調レベル設定に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
		return hResult;
	}


	//デバイスへの入力制御開始
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
	//---各種宣言---//
	int nCounter;

	//---データの更新---//
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

// ゲームパッド問い合わせ用コールバック関数
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
	//---各種宣言---//
	HRESULT hResult;

	//DirectInputオブジェクト生成
	hResult = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&Manager, nullptr);
	if (FAILED(hResult)){
		MessageBox(handle, TEXT("DirectInputオブジェクト生成に失敗しました"), TEXT("初期化エラー"), MB_ICONSTOP | MB_OK);
		Uninitialize();
		return hResult;
	}


	//キーボードの初期化
	hResult = keyboard.Initialize(handle, Manager);
	if (FAILED(hResult)){
		Uninitialize();
		return hResult;
	}

	//マウスの初期化
	hResult = mouse.Initialize(handle, Manager);
	if (FAILED(hResult))
	{
		Uninitialize();
		return hResult;
	}

	// ゲームパッド初期化
	//////////////////////////////////////////////////////////////////////////////////////

	g_dwJoystick = 0;
	hResult = Input::GetManagerDat()->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hResult)) {
		MessageBoxA(NULL, "ゲームパッド オブジェクト生成に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hResult;
	}
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		// データ フォーマット設定
		hResult = g_pDIDeviceJoy[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hResult)) {
			MessageBoxA(NULL, "ゲームパッド データ フォーマット設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
			return hResult;
		}
		// 協調レベルの設定
		hResult = g_pDIDeviceJoy[i]->SetCooperativeLevel(handle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hResult)) {
		
			MessageBoxA(NULL, "ゲームパッド協調レベル設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
			return hResult;
		}
		// デバイス設定
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



// ここからゲームパッド

// ゲームパッド数取得
DWORD DIGetJoyCount()
{
	return g_dwJoystick;
}

// ゲームパッド状態取得
DIJOYSTATE *DIGetJoy(DWORD dwJoy)
{

	if (!g_joyOn) {
		DIJOYSTATE joy;
		return &joy;
	}
	if (dwJoy >= g_dwJoystick) return NULL;
	return &g_diJoyState[dwJoy];
}

// ゲームパッドX軸取得
LONG DIGetJoyX(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lX;
}

// ゲームパッドY軸取得
LONG DIGetJoyY(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lY;
}

// ゲームパッドZ軸取得
LONG DIGetJoyZ(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lZ;
}

// ゲームパッド ボタン押下状態取得
bool DIGetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (!g_joyOn) return false;
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyState[dwJoy].rgbButtons)) return false;
	return (g_diJoyState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// ゲームパッド ボタン トリガ状態取得
bool DIGetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (!g_joyOn) return false;
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyTriggerState[dwJoy].rgbButtons)) return false;
	return (g_diJoyTriggerState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// ゲームパッド ボタン リリース状態取得
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
	if (!g_joyOn) return 0.0f;			// 使われてないならここで終了

	static const float adjNum = 1024;	// 計算用に値を小さくした結果のMAX値
	LONG retNum = 0;
	switch (dwStk) {
	case PAD_ST_L_X: retNum = DIGetJoy(0)->lX;
		break;
	case PAD_ST_L_Y: retNum = DIGetJoy(0)->lY;
		break;
	case PAD_ST_R_X: retNum = DIGetJoy(0)->lZ;
		retNum += CALC_TO_FLOAT_MAX;	// 初期が0~なので
		break;
	case PAD_ST_R_Y: retNum = DIGetJoy(0)->lRz;
		retNum += CALC_TO_FLOAT_MAX;	// 初期が0~なので
		break;
	case PAD_L2: retNum = DIGetJoy(0)->lRx;
		break;
	case PAD_R2: retNum = DIGetJoy(0)->lRy;
		break;
	default:
#ifdef _DEBUG
		MessageBox(NULL, "DigetPadStickの引数が間違っています", "パッドエラー", MB_OK);
#endif // _DEBUG
		break;
	}
	if (abs(retNum) < RANGE_ZERO_STICK) {	// 値が小さすぎるならデフォルト値を返す
		if (!adj) return 0;		// 値が0からなら
		if (adj) return 1.0f;		// 値がマイナスからなら
	}
	float calcOut = 0.0f;	// 計算用変数
	retNum = retNum >> 5;	// 1024に値を小さくしてから計算	


	// 正の値の最大値が1足りなくなるため割合の最大値が0.99になってしまうのでadjNum-1の値ならMAXとして扱う
	if (retNum == adjNum - 1) retNum = (LONG)adjNum;

	calcOut = (float)retNum / 1024;	//0.0f~1.0fに(速度は無視)


	if (adj) {	// 指定されたならマイナスからではなくゼロからの値に
		calcOut += 1.0f;
	}

	return calcOut;
}