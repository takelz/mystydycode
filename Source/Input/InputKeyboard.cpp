#include "InputKeyboard.h"

//----------
// グローバル変数
//----------
static BYTE g_keyTable[256];
static BYTE	g_keyTrigger[256];	// キーボードのトリガ状態を保持するワーク
static BYTE	g_keyRelease[256];	// キーボードのリリース状態を保持するワーク

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
