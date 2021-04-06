#include"PostNoiseWave.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"
#include"NormalRender.h"
#include"DepthBuffer.h"

using namespace DirectX;

#define NOISE_SPEED (0.03f)
#define END_TIME	(0.02f)


void PostNoiseWave::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	graphic.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(graphic.pTexture);
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(0.0f, 0.0f, 0.0f);
	polygon->SetScale(1.0f, 1.0f);

	// バッファ作成
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(Buffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &constBuffer);

	setBuffer.time.x = 0.0f;
	setBuffer.time.y = 0.0f;

}


void PostNoiseWave::Render() {
	// effectが終わっていたら
	if (setBuffer.time.y <= 0.0f) {
		setBuffer.time.y = 0.0f;
		return;
	}

	setBuffer.time.x += NOISE_SPEED;

	if(setBuffer.time.y > 0.0f )setBuffer.time.y -= END_TIME;
	setBuffer.damy.x = setBuffer.damy.y = 0.0f;

	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);

	if (setBuffer.time.x > 10.0f) setBuffer.time.x = 10.0f;
	// レンダー切替
	//GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	
	GetDX11DeviceContext()->OMSetRenderTargets(2, &graphic.pRenderTarget, nullptr);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLOCKNOISE);
	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	// ノイズテクスチャ作成
	polygon->Draw();
	// 
	GetDX11DeviceContext()->PSSetShaderResources(1, 1, &graphic.pTexture);


	// メインのレンダーにも描き込む
	GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	polygon->SetTexture(graphic.pTexture);
	polygon->Draw();

}

void PostNoiseWave::TriggerOn() {
	//effect起動
	setBuffer.time.y = 1.0f;
	setBuffer.time.x = 0.0f;
}