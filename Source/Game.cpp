//*****************************************************************************
//! @file	game.cpp
//! @brief	ゲーム処理
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
//!	@brief	ゲーム初期処理
//!	@param[in] hinst インスタンス値
//!	@param[in] hwnd ウインドウハンドル値
//!	@param[in] width ウインドウサイズ幅（補正済みの値）
//!	@param[in] height ウインドウサイズ高さ（補正済みの値）
//!	@param[in] fullscreen フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	HRESULT hr = E_FAIL;
	bool result = false;

	// アセットディレクトリ探索
	char dir[MAX_PATH];
	do {
		GetCurrentDirectory(sizeof(dir), dir);
		PathAppend(dir, "Assets");
		if (PathFileExists(dir)) {
			break;
		}
	} while (SetCurrentDirectory("../"));

	// DX11初期処理
	hr = DX11Init(hwnd, width, height, fullscreen);
	if (FAILED(hr)) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	g_hwnd = hwnd;

	// テクスチャ
	DX11TextureManager::Create();
	// ポリゴン
	DX11PolygonInterface::Create();
	// 初期化
	Plane::InitializeVertex();
	// 3D描画デバッグ
	debug::InitializeDrawObject();
	// デバッグ初期化
	InitializeDebugUtility();
	// 入力初期化
	//InitializeInputUtility(1);
	Input::Initialize(hinst, hwnd);

	// サウンド初期化
	InitSound();

	// シェーダー準備
	ShaderSelector::Get()->Setup();
	TextureShaderSelector::Get()->Setup();


	GameObjectList::Get()->Setup();
	SceneManager::Get()->LoadScene((SceneNum)1);

	Camera::Instance()->Init(1.0f, DEFAULT_CAMERA_FAR,
		XMConvertToRadians(60), SCREEN_X, SCREEN_Y,
		XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 0));

	//CScene::CurrentInit();

	// カーソルを無効

	ShowCursor(false);
	// フェード
	fade.Init();

	EffekseerManager::Get();

	return	true;
}
static bool flg = false;
#include"Src_Player/Player.h"
//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
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
	// デバッグ用
	if(CCC()) CCC()->Update();
	Camera::Instance()->Update();

}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
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

	// レンダリング前処理
	DX11BeforeRender(ClearColorRGBA);

	//pGraphics->EnableZBuffer(true);
	//GetDX11Light()->SetEnable(true);
	GetDX11Light()->Update();

	// シーン描画
	GameObjectList::Get()->Draw();
	EffekseerManager::Get()->Draw();
	GameObjectList::Get()->LateUpdate();

	fade.Draw();
	// デバッグ描画
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
	// レンダリング後処理
	DX11AfterRender();
}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameMain()
{
	Input::Update();
	//UpdateInputUtility();
	DbgMenu->Update();
	if (!DbgMenu->IsOpenMenu()) {
		GameUpdate();	// 更新
	}
	GameRender();		// 描画
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameExit()
{
	// シーン終了処理


	// システム終了処理
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
