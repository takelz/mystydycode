
#include "../Shader/GraphicTexture.h"
#include "RenderTargetManager.h"
#include <Shader/ShadowWrite.h>
#include <Shader/NormalRender.h>
#include <Shader/DepthBuffer.h>
#include <Shader/PostDistortionObject.h>

RenderTargetManager::RenderTargetManager() {
	currentRenderPath = 0;
}

 
void RenderTargetManager::OrderRender(int path){
	static float clearColor[4] = { 1.0f, 0.3f, 1.0f, 1.0f };
	ID3D11DepthStencilView* pDSV = GetDX11Graphics()->GetDepthStencilView();
	ID3D11RenderTargetView* pRTV = GetDX11Graphics()->GetRenderTargetView();
	currentRenderPath = path;
	switch (path)
	{
	case 1:	// 影描画用のテクスチャ
		GetDX11DeviceContext()->OMSetRenderTargets(1, &ShadowWrite::Get()->graphic.pRenderTarget, pDSV);
		GetDX11DeviceContext()->ClearRenderTargetView(ShadowWrite::Get()->graphic.pRenderTarget, clearColor);
		GetDX11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH , 1.0f, 0);
		break;
	case 2:	// 通常の深度バッファ用テクスチャ
		GetDX11DeviceContext()->OMSetRenderTargets(1, &DepthBuffer::Get()->graphic.pRenderTarget, pDSV);
		GetDX11DeviceContext()->ClearRenderTargetView(DepthBuffer::Get()->graphic.pRenderTarget, clearColor);
		GetDX11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		break;
	case 3: // 通常のテクスチャ
		GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, pDSV);
		GetDX11DeviceContext()->ClearRenderTargetView(NormalRender::Get()->graphic.pRenderTarget, clearColor);
		GetDX11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH , 1.0f, 0);
		break;
	case 4: // 通常
		GetDX11DeviceContext()->OMSetRenderTargets(1, &pRTV, pDSV);
		GetDX11DeviceContext()->ClearRenderTargetView(pRTV, clearColor);
		GetDX11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		break;
	case 5: // 歪み用
		clearColor[0] = clearColor[1] = clearColor[2] = clearColor[3] = 0.0f;
		GetDX11DeviceContext()->OMSetRenderTargets(1, &PostDistortionObject::Get()->distNormal.pRenderTarget, pDSV);
		GetDX11DeviceContext()->ClearRenderTargetView(PostDistortionObject::Get()->distNormal.pRenderTarget, clearColor);
		// 消さない
		//GetDX11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		break;
	default:
		break;
	}
}