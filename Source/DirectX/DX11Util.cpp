/**
 * @file DX11Util.cpp
 * @brief DX11�ėp
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
 * @brief DirectX������
 * @param[in] hWnd �E�B���h�E�n���h��
 * @param[in] width �E�B���h�E�T�C�Y
 * @param[in] height �E�B���h�E����
 * @param[in] fullscreen �t���X�N���[���w��
 * @return ��������
 */
HRESULT DX11Init(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
	if (g_pDXGraphics != NULL || g_pDXLight != NULL) {
		return E_FAIL;
	}

	// �O���t�B�b�N
	g_pDXGraphics = new DX11Graphics();
	if (FAILED(g_pDXGraphics->Init(hWnd, width, height, fullscreen))) {
		return E_FAIL;
	}

	// ���C�g
	g_pDXLight = new DX11Light();
	if (FAILED(g_pDXLight->Init())) {
		return E_FAIL;
	}

	return S_OK;
}

/**
 * @brief �I������
 */
void DX11Uninit()
{
	g_pDXLight->Uninit();
	SAFE_DELETE(g_pDXLight);
	g_pDXGraphics->Exit();
	SAFE_DELETE(g_pDXGraphics);
}

/**
 * @brief �`��N���A
 * @param[in] ClearColorRBGA ��ʃN���A�F
 */
void DX11BeforeRender(const float ClearColorRGBA[])
{
	g_pDXGraphics->RenderCleanup(ClearColorRGBA);
}

/**
 * @brief �`��
 */
void DX11AfterRender()
{
	g_pDXGraphics->GetSwapChain()->Present(1, 0);
}

// EOF