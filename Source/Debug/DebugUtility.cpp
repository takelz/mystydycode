#include "DebugUtility.h"

void InitializeDebugUtility()
{
	DebugText::Create();
	DebugMenu::Create();
}

void TerminateDebugUtility()
{
	DebugMenu::Delete();
	DebugText::Delete();
}

// EOF