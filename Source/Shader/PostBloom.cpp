#include"PostBloom.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"
#include"NormalRender.h"
#include"PostBlur.h"

using namespace DirectX;


void PostBloom::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	brightTex.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(0.0f, 0.0f, 0.0f);
	polygon->SetScale(1.0f, 1.0f);

	lastResult.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);


	// バッファ作成
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(Buffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &constBuffer);
}

void PostBloom::Render() {
	float cl[4] = { 1.0f,0.7f,1.0f,1.0f };
	setBuffer.power.x = 0.45f;
	setBuffer.power.y = 0.45f;
	setBuffer.damy.x = setBuffer.damy.y = 0.0f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);
	
	// Bloom作成用ターゲット切替
	GetDX11DeviceContext()->ClearRenderTargetView(brightTex.pRenderTarget, cl);
	GetDX11DeviceContext()->OMSetRenderTargets(2, &brightTex.pRenderTarget, nullptr);

	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_GETBRIGHT);
	polygon->Draw();			// これでbrightTexTargetのテクスチャは輝度値のものに

	Blur::Get()->BlurTexture(&brightTex);	// ぼかす。与えたテクスチャはぼけて帰ってくる

	// ぼかすステージ
	GetDX11DeviceContext()->ClearRenderTargetView(lastResult.pRenderTarget, cl);
	GetDX11DeviceContext()->OMSetRenderTargets(3, &lastResult.pRenderTarget, nullptr);

	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLOOM);
	GetDX11DeviceContext()->PSSetShaderResources(1, 1, &brightTex.pTexture);
	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	polygon->Draw();


	// メインターゲットに
	// 加算用テクスチャセット、ぼけた加算テクスチャを元のテクスチャに重ねる
	GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	polygon->SetTexture(lastResult.pTexture);
	polygon->Draw();


	//polygon->Draw();

	
}