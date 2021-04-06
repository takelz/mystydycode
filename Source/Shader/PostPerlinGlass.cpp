#include"PostPerlinGlass.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"
#include"NormalRender.h"
#include"DepthBuffer.h"


using namespace DirectX;


void PostPerlinGlass::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	noise.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(noise.pTexture);
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(0.0f, 0.0f, 0.0f);
	polygon->SetScale(1.0f, 1.0f);

	noiseR.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	noiseG.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	noiseB.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);

	// バッファ作成
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(Buffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &constBuffer);

	setBuffer.damy.x = setBuffer.damy.y = 0.0f;

	renderGlassFilter = false;
}


void PostPerlinGlass::Render() {
	if (!renderGlassFilter)return;

	
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);
	// レンダー切替
	//GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	

	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	// それぞれの色情報に法線になるランダムな情報を入れる
	// 赤
	GetDX11DeviceContext()->OMSetRenderTargets(2, &noiseR.pRenderTarget, nullptr);
	setBuffer.time.x = 13.2f;
	setBuffer.time.y = 22.1f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_PERLIN_NOISE);
	polygon->Draw();
	// 緑
	GetDX11DeviceContext()->OMSetRenderTargets(2, &noiseG.pRenderTarget, nullptr);
	setBuffer.time.x = 22.0f;
	setBuffer.time.y = 12.0f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_PERLIN_NOISE);
	polygon->Draw();



	// 描き込む
	GetDX11DeviceContext()->OMSetRenderTargets(2, &noise.pRenderTarget, nullptr);
	GetDX11DeviceContext()->PSSetShaderResources(1, 1, &noiseR.pTexture);
	GetDX11DeviceContext()->PSSetShaderResources(2, 1, &noiseG.pTexture);
	GetDX11DeviceContext()->PSSetShaderResources(3, 1, &noiseB.pTexture);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_GLASS_FILTER);
	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	
	polygon->Draw();

	GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	polygon->SetTexture(noise.pTexture);
	polygon->Draw();

	if (Input::GetKey(DIK_7, INPUT::HOLD)) {
		polygon->SetTexture(noiseR.pTexture);
		polygon->Draw();
	}

}