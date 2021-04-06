#pragma once


#define    DIRECTINPUT_VERSION    0x0800
#include <dinput.h>    // DirectInput�o�[�W�����̐錾
#include <DirectXMath.h>
#include <array>
#include "Mouse.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



#define PUSHTYPE        Input::PUSH_TYPE
#define INPUT Input


enum {
	PAD_SQUARE,		// ��
	PAD_CROSS,		// �~
	PAD_CIRCLE,		// ��
	PAD_TRIANGLE,	// ��
	PAD_L1,
	PAD_R1,
	PAD_L2,		// �{�^���ƃX�e�B�b�N�����\
	PAD_R2,		// �{�^���ƃX�e�B�b�N�����\
	PAD_SHARE,
	PAD_START,
	PAD_L3,
	PAD_R3,
	PAD_HOME,	//�@�g��Ȃ�����
	PAD_TOUCH,
	PAD_ST_L_X,
	PAD_ST_L_Y,
	PAD_ST_R_X,
	PAD_ST_R_Y,
	PAD_UP,		// �\���L�[��@������
	PAD_DOWN,	// �\���L�[��  ������
	PAD_RIGHT,	// �\���L�[�E  ������
	PAD_LEFT,	// �\���L�[��  ������
};



//�������N���X�錾������//
class Keyboard
{
private:
	LPDIRECTINPUTDEVICE8 Device;    //�f�o�C�X
	std::array<BYTE, 256> Current;    //���݂̏��
	std::array<BYTE, 256> Preview;    //�O��̏��
	std::array<BYTE, 256> Trigger;    //�g���K���
	std::array<BYTE, 256> Release;    //�����[�X���

public:
	HRESULT Initialize(HWND, LPDIRECTINPUT8);
	void Uninitialize(void);
	void Update(void);

	bool GetHold(DWORD);
	bool GetRelease(DWORD);
	bool GetTrigger(DWORD);
};

//�������N���X�錾������//
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
// �{�^���̎w��͗񋓑̂́@PAD_ ~~
bool DIGetPadTrigger(DWORD dwBtn);
// �{�^���̎w��͗񋓑̂́@PAD_ ~~
bool DIGetPadButton(DWORD dwBtn);
// �{�^���̎w��͗񋓑̂́@PAD_ ~~
bool DIGetPadRelease(DWORD dwBtn);


// �p�b�h�̃X�e�B�b�N�̒l��Ԃ��B�l��-1.0f~1.0f
// PAD_NUM_ADJ �̒萔���Ԗڂ̈����ɓ�����0~2.0f�̒l���Ԃ��Ă���B
// �{�^���̎w��͗񋓑̂� PAD_~~ �Ŏw�肷��B
// R2��L2���Ή��B 
float DIGetPadStick(DWORD dwStk, DWORD adj);

// �p�b�h�̃X�e�B�b�N�̒l��Ԃ��B�l��-1.0f~1.0f(��������݂̂̏ꍇ)
// �{�^���̎w��͗񋓑̂� PAD_~~ �Ŏw�肷��B
// R2��L2���Ή��B 
float DIGetPadStick(DWORD dwStk);