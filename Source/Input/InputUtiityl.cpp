#include "InputUtility.h"
#include "InputKeyboard.h"
#include "InputGamePad.h"
#include <Debug/DebugMemory.h>

//----------
// ƒOƒ[ƒoƒ‹•Ï”
//----------
InputInterface *g_pInputInterface = NULL;
int g_inputInterfaceNum = 0;

bool InitializeInputUtility(int num)
{
	g_inputInterfaceNum = num;
	g_pInputInterface = NEW InputInterface[g_inputInterfaceNum];

	if (FAILED(InitializeGamePad(1))) {
		return false;
	}

	return true;
}

void TerminateInputUtility()
{
	TerminateGamePad();
	SAFE_DELETE_ARRAY(g_pInputInterface);
}

void UpdateInputUtility()
{
	UpdateKeyboard();
	UpdateGamePad();
	for (int i = 0; i < g_inputInterfaceNum; ++i) {
		g_pInputInterface[i].Update();
	}
}

InputInterface *GetInputInterface(int id)
{
	return &g_pInputInterface[id];
}