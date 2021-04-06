/**
 * @file DX11Util.cpp
 * @brief DX11汎用
 */
#include "DX11Util.h"
#include "DX11Graphics.h"
#include "DX11Light.h"
#include <Debug/DebugMemory.h>

DX11Graphics *g_pDXGraphics = NULL;
DX11Light *g_pDXLight = NULL;


//! @name Get function
/// @{
ID3D11Device* GetDX11Device()
{
	return g_pDXGraphics->GetDevice();
}
ID3D11DeviceContext* GetDX11DeviceContext()
{
	return g_pDXGraphics->GetDeviceContext();
}
DX11Graphics *GetDX11Graphics()
{
	return g_pDXGraphics;
}
DX11Light *GetDX11Light()
{
	return g_pDXLight;
}
/// @}

/**
 * @brief DirectX初期化
 * @param[in] hWnd ウィンドウハンドル
 * @param[in] width ウィンドウサイズ
 * @param[in] height ウィンドウ高さ
 * @param[in] fullscreen フルスクリーン指定
 * @return 処理結果
 */
HRESULT DX11Init(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
	if (g_pDXGraphics != NULL || g_pDXLight != NULL) {
		return E_FAIL;
	}

	// グラフィック
	g_pDXGraphics = new DX11Graphics();
	if (FAILED(g_pDXGraphics->Init(hWnd, width, height, fullscreen))) {
		return E_FAIL;
	}

	// ライト
	g_pDXLight = new DX11Light();
	if (FAILED(g_pDXLight->Init())) {
		return E_FAIL;
	}

	return S_OK;
}

/**
 * @brief 終了処理
 */
void DX11Uninit()
{
	g_pDXLight->Uninit();
	SAFE_DELETE(g_pDXLight);
	g_pDXGraphics->Exit();
	SAFE_DELETE(g_pDXGraphics);
}

/**
 * @brief 描画クリア
 * @param[in] ClearColorRBGA 画面クリア色
 */
void DX11BeforeRender(const float ClearColorRGBA[])
{
	g_pDXGraphics->RenderCleanup(ClearColorRGBA);
}

/**
 * @brief 描画
 */
void DX11AfterRender()
{
	g_pDXGraphics->GetSwapChain()->Present(1, 0);
}

// EOF