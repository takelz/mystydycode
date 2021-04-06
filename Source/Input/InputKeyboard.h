#ifndef __INPUT_KEYBOARD_H__
#define __INPUT_KEYBOARD_H__

#include <Windows.h>

//----------
// ç\ë¢ëÃÅAóÒãìå^
//----------
enum VirtualKeyKind
{
	VK_A = 'A',
	VK_B = 'B',
	VK_C = 'C',
	VK_D = 'D',
	VK_E = 'E',
	VK_F = 'F',
	VK_G = 'G',
	VK_H = 'H',
	VK_I = 'I',
	VK_J = 'J',
	VK_K = 'K',
	VK_L = 'L',
	VK_M = 'M',
	VK_N = 'N',
	VK_O = 'O',
	VK_P = 'P',
	VK_Q = 'Q',
	VK_R = 'R',
	VK_S = 'S',
	VK_T = 'T',
	VK_U = 'U',
	VK_V = 'V',
	VK_W = 'W',
	VK_X = 'X',
	VK_Y = 'Y',
	VK_Z = 'Z',

	VK_0 = '0',
	VK_1 = '1',
	VK_2 = '2',
	VK_3 = '3',
	VK_4 = '4',
	VK_5 = '5',
	VK_6 = '6',
	VK_7 = '7',
	VK_8 = '8',
	VK_9 = '9',
};


//----------
// ä÷êî
//----------
void UpdateKeyboard();

bool IsPressKeyboard(UINT nVertKey);
bool IsTriggerKeyboard(UINT nVertKey);
bool IsReleaseKeyboard(UINT nVertKey);

#endif // __INPUT_KEYBOARD_H__