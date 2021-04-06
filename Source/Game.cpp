//*****************************************************************************
//! @file	game.cpp
//! @brief	�Q�[������
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include <DirectX/DrawObject.h>
#include <DirectX/DX11Polygon.h>
#include <DirectX/WinFont.h>
#include <Model/ModelObject.h>
//#include <Input/InputUtility.h>
//#include <Input/InputGamePad.h>
#include <Debug/DebugUtility.h>
#include <Sound/Sound.h>
#include "Game.h"
#include "Camera.h"
#include "DXg11/GameObjectList.h"
#include "DXg11/SceneManager.h"
#include "DXg11/Input.h"
#include "Fade.h"
#include "EffekseerManager.h"

using namespace DirectX;

#define DEFAULT_CAMERA_FAR (10000.0f)

 static HWND g_hwnd;
 Fade fade;

//==============================================================================
//!	@brief	�Q�[����������
//!	@param[in] hinst �C���X�^���X�l
//!	@param[in] hwnd �E�C���h�E�n���h���l
//!	@param[in] width �E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param[in] height �E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param[in] fullscreen �t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	HRESULT hr = E_FAIL;
	bool result = false;

	// �A�Z�b�g�f�B���N�g���T��
	char dir[MAX_PATH];
	do {
		GetCurrentDirectory(sizeof(dir), dir);
		PathAppend(dir, "Assets");
		if (PathFileExists(dir)) {
			break;
		}
	} while (SetCurrentDirectory("../"));

	// DX11��������
	hr = DX11Init(hwnd, width, height, fullscreen);
	if (FAILED(hr)) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	g_hwnd = hwnd;

	// �e�N�X�`��
	DX11TextureManager::Create();
	// �|���S��
	DX11PolygonInterface::Create();
	// ������
	Plane::InitializeVertex();
	// 3D�`��f�o�b�O
	debug::InitializeDrawObject();
	// �f�o�b�O������
	InitializeDebugUtility();
	// ���͏�����
	//InitializeInputUtility(1);
	Input::Initialize(hinst, hwnd);

	// �T�E���h������
	InitSound();

	// �V�F�[�_�[����
	ShaderSelector::Get()->Setup();
	TextureShaderSelector::Get()->Setup();


	GameObjectList::Get()->Setup();
	SceneManager::Get()->LoadScene((SceneNum)1);

	Camera::Instance()->Init(1.0f, DEFAULT_CAMERA_FAR,
		XMConvertToRadians(60), SCREEN_X, SCREEN_Y,
		XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 0));

	//CScene::CurrentInit();

	// �J�[�\���𖳌�

	ShowCursor(false);
	// �t�F�[�h
	fade.Init();

	EffekseerManager::Get();

	return	true;
}
static bool flg = false;
#include"Src_Player/Player.h"
//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate()
{
	//static RECT rect;	
	//GetWindowRect(GetDesktopWindow(), &rect);
	//SetCursorPos((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

	if (Input::GetKey(DIK_8, Input::TRIGGER)) { flg = !flg; }
	
	if (!flg) {
		SceneManager::Get()->SceneTrance();

		GameObjectList::Get()->Update();
		EffekseerManager::Get()->GetManager()->Update();
		fade.Update();
	}
	// �f�o�b�O�p
	if(CCC()) CCC()->Update();
	Camera::Instance()->Update();

}

//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender()
{
	DX11Graphics *pGraphics = GetDX11Graphics();
	float ClearColorRGBA[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	ModelObject::Transform transform = {
		XMMatrixIdentity(),
		XMLoadFloat4x4(&Camera::Instance()->GetCameraMatrix()),
		XMLoadFloat4x4(&Camera::Instance()->GetProjectionMatrix()),
	};

	// �����_�����O�O����
	DX11BeforeRender(ClearColorRGBA);

	//pGraphics->EnableZBuffer(true);
	//GetDX11Light()->SetEnable(true);
	GetDX11Light()->Update();

	// �V�[���`��
	GameObjectList::Get()->Draw();
	EffekseerManager::Get()->Draw();
	GameObjectList::Get()->LateUpdate();

	fade.Draw();
	// �f�o�b�O�`��
	/*
	{
		// 3D
		pGraphics->EnableZBuffer(true);
		XMFLOAT4X4 viewproj;
		DirectX::XMStoreFloat4x4(&viewproj, transform.view * transform.proj);
		DirectX::debug::FlushDrawObject(viewproj);

		// 2D
		pGraphics->EnableZBuffer(false);
		DbgMenu->Draw();
		DbgText->Draw();
	}

	*/
	// �����_�����O�㏈��
	DX11AfterRender();
}

//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameMain()
{
	Input::Update();
	//UpdateInputUtility();
	DbgMenu->Update();
	if (!DbgMenu->IsOpenMenu()) {
		GameUpdate();	// �X�V
	}
	GameRender();		// �`��
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameExit()
{
	// �V�[���I������


	// �V�X�e���I������
	UninitSound();
	//TerminateInputUtility();
	Input::Uninitialize();
	TerminateDebugUtility();
	DirectX::debug::TerminateDrawObject();
	DX11PolygonInterface::Delete();
	DX11TextureManager::Delete();
	DX11Uninit();
}

XMFLOAT2 GetMousePos() {
	POINT lpPoint;
	XMFLOAT2 mousePos;

	GetCursorPos(&lpPoint);
	ScreenToClient(g_hwnd, &lpPoint);

	mousePos.x = (float)lpPoint.x;
	mousePos.y = (float)lpPoint.y;

	return mousePos;
}

HWND GetHwnd() {
	return g_hwnd;
}



//******************************************************************************
//	End of file.
//******************************************************************************
