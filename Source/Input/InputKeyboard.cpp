#include "InputKeyboard.h"

//----------
// �O���[�o���ϐ�
//----------
static BYTE g_keyTable[256];
static BYTE	g_keyTrigger[256];	// �L�[�{�[�h�̃g���K��Ԃ�ێ����郏�[�N
static BYTE	g_keyRelease[256];	// �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N

void UpdateKeyboard()
{
	BYTE keyTable[256];
	GetKeyboardState(keyTable);
	for (int i = 0; i < 256; ++i) {
		g_keyTrigger[i] = (g_keyTable[i] ^ keyTable[i]) & keyTable[i];
		g_keyRelease[i] = (g_keyTable[i] ^ keyTable[i]) & ~keyTable[i];
		g_keyTable[i] = keyTable[i];
	}
}

bool IsPressKeyboard(UINT nVertKey)
{
	return (g_keyTable[nVertKey] & 0x80) > 0;
}

bool IsTriggerKeyboard(UINT nVertKey)
{
	return (g_keyTrigger[nVertKey] & 0x80) > 0;
}

bool IsReleaseKeyboard(UINT nVertKey)
{
	return (g_keyRelease[nVertKey] & 0x80) > 0;
}
