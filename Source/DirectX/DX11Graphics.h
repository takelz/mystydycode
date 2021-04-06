/**
 * @file DX11Graphics.h
 * @brief DX11グラフィックス
 */
#ifndef __DX11_GRAPHICS_H__
#define __DX11_GRAPHICS_H__

#include "DX11Util.h"

/**
 * @brief DirectXデバイス管理
 */
class DX11Graphics
{
private:
	ID3D11Device*				m_pDevice;				// DIRECT3DDEVICE11デバイス
	ID3D11DeviceContext*		m_pDeviceContext;		// DIRECT3DDEVICE11デバイスコンテキスト
	IDXGISwapChain*				m_pSwapChain;			// スワップチェイン

	ID3D11RenderTargetView*		m_pRenderTargetView;	// レンダーターゲットビュー
	ID3D11Texture2D*			m_pDepthStencilBuffer;	// Ｚバッファ、ステンシルバッファ
	ID3D11DepthStencilState*	m_pDepthStencilState;	// Ｚバッファ、ステンシルステート
	ID3D11DepthStencilView*		m_pDepthStencilView;	// Ｚバッファ、ステンシルビュー
	ID3D11SamplerState*			m_pSamplerState;		///< サンプラ―ステート
	ID3D11RasterizerState*		m_pRasterState;			// ラスターステータス
	ID3D11BlendState*			m_pBlendState;			///< ブレンドステート

	D3D_DRIVER_TYPE				m_driverType;			// ドライバタイプ
	D3D_FEATURE_LEVEL			m_featureLevel;			// 機能レベル
	UINT						m_width;				// バックバッファＸサイズ
	UINT						m_height;				// バックバッファＹサイズ


public:
	DX11Graphics();
	~DX11Graphics();

	// 初期処理
	HRESULT Init(HWND hWnd, UINT Width, UINT Height, bool fullscreen = false);
	// 終了処理
	void Exit();
	// 描画クリア
	void RenderCleanup(const float color[]);


	void EnableZBuffer(bool enable);

	// 取得
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	IDXGISwapChain* GetSwapChain() const;
	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;
};

#endif // __DX11_GRAPHICS_H__