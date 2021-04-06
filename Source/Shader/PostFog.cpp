#include"PostFog.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"
#include"NormalRender.h"
#include"DepthBuffer.h"

using namespace DirectX;


void PostFog::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	graphic.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(graphic.pTexture);
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(0.0f, 0.0f, 0.0f);
	polygon->SetScale(1.0f, 1.0f);

}


void PostFog::Render() {
	

	
	// レンダー切替
	//GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	
	GetDX11DeviceContext()->OMSetRenderTargets(2, &graphic.pRenderTarget, nullptr);
	GetDX11DeviceContext()->PSSetShaderResources(1, 1, &DepthBuffer::Get()->graphic.pTexture);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_FOG);
	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	polygon->Draw();

	// メインのレンダーにも描き込む
	GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	polygon->SetTexture(graphic.pTexture);
	polygon->Draw();

}