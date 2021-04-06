/**
 * @file DX11Graphics.h
 * @brief DX11�O���t�B�b�N�X
 */
#ifndef __DX11_GRAPHICS_H__
#define __DX11_GRAPHICS_H__

#include "DX11Util.h"

/**
 * @brief DirectX�f�o�C�X�Ǘ�
 */
class DX11Graphics
{
private:
	ID3D11Device*				m_pDevice;				// DIRECT3DDEVICE11�f�o�C�X
	ID3D11DeviceContext*		m_pDeviceContext;		// DIRECT3DDEVICE11�f�o�C�X�R���e�L�X�g
	IDXGISwapChain*				m_pSwapChain;			// �X���b�v�`�F�C��

	ID3D11RenderTargetView*		m_pRenderTargetView;	// �����_�[�^�[�Q�b�g�r���[
	ID3D11Texture2D*			m_pDepthStencilBuffer;	// �y�o�b�t�@�A�X�e���V���o�b�t�@
	ID3D11DepthStencilState*	m_pDepthStencilState;	// �y�o�b�t�@�A�X�e���V���X�e�[�g
	ID3D11DepthStencilView*		m_pDepthStencilView;	// �y�o�b�t�@�A�X�e���V���r���[
	ID3D11SamplerState*			m_pSamplerState;		///< �T���v���\�X�e�[�g
	ID3D11RasterizerState*		m_pRasterState;			// ���X�^�[�X�e�[�^�X
	ID3D11BlendState*			m_pBlendState;			///< �u�����h�X�e�[�g

	D3D_DRIVER_TYPE				m_driverType;			// �h���C�o�^�C�v
	D3D_FEATURE_LEVEL			m_featureLevel;			// �@�\���x��
	UINT						m_width;				// �o�b�N�o�b�t�@�w�T�C�Y
	UINT						m_height;				// �o�b�N�o�b�t�@�x�T�C�Y


public:
	DX11Graphics();
	~DX11Graphics();

	// ��������
	HRESULT Init(HWND hWnd, UINT Width, UINT Height, bool fullscreen = false);
	// �I������
	void Exit();
	// �`��N���A
	void RenderCleanup(const float color[]);


	void EnableZBuffer(bool enable);

	// �擾
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	IDXGISwapChain* GetSwapChain() const;
	ID3D11RenderTargetView* GetRenderTargetView() const;
	ID3D11DepthStencilView* GetDepthStencilView() const;
};

#endif // __DX11_GRAPHICS_H__