//*****************************************************************************
//!	@file	game.h
//!	@brief	�Q�[������
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include <DirectX/DX11Util.h>
#include "DXg11/Vector.h"

#define		SCREEN_X		1200
#define		SCREEN_Y		750
#define		FULLSCREEN      0

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
void GameMain();
void GameExit();
void GameUpdate();
void GameRender();


DirectX::XMFLOAT2 GetMousePos();
HWND GetHwnd();

//******************************************************************************
//	End of file.
//******************************************************************************
