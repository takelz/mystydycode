#ifndef __INPUT_GAMEPAD_H__
#define __INPUT_GAMEPAD_H__

#include <windows.h>
#pragma comment(lib, "winmm.lib")

enum GamePadButtonKind
{
	PAD_BUTTON_A = JOY_BUTTON1,
	PAD_BUTTON_B = JOY_BUTTON2,
	PAD_BUTTON_X = JOY_BUTTON3,
	PAD_BUTTON_Y = JOY_BUTTON4,

	PAD_BUTTON_L1 = JOY_BUTTON5,
	PAD_BUTTON_L2 = JOY_BUTTON27,
	PAD_BUTTON_L3 = JOY_BUTTON9,
	PAD_BUTTON_R1 = JOY_BUTTON6,
	PAD_BUTTON_R2 = JOY_BUTTON28,
	PAD_BUTTON_R3 = JOY_BUTTON10,

	PAD_BUTTON_UP = JOY_BUTTON29,
	PAD_BUTTON_DOWN = JOY_BUTTON30,
	PAD_BUTTON_LEFT = JOY_BUTTON31,
	PAD_BUTTON_RIGHT = JOY_BUTTON32,

	PAD_BUTTON_START = JOY_BUTTON8,
	PAD_BUTTON_SELECT = JOY_BUTTON7,
};
enum GamePadAxisKind
{
	PAD_AXIS_LX,
	PAD_AXIS_LY,
	PAD_AXIS_LZ,
	PAD_AXIS_RX,
	PAD_AXIS_RY,
	PAD_AXIS_RZ,
	PAD_AXIS_MAX
};


typedef void(*GamePadConnectCallback)(bool);

HRESULT InitializeGamePad(UINT maxPad);
void TerminateGamePad();
void UpdateGamePad();

void SetGamePadConnectCallback(GamePadConnectCallback func);

int GetGamePadConnectNum();
bool IsPressGamePad(UINT id, GamePadButtonKind button);
float GetAxisGamePad(UINT id, GamePadAxisKind axis);


#endif // __INPUT_GAMEPAD_H__