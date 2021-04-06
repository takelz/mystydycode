
#include"PostBlur.h"
#include"ShaderSelector.h"
#include"NormalRender.h"
#include"../DXg11/GameObjectBase.h"

using namespace DirectX;


void Blur::Create() {
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };

	polygon = new DX11Polygon(DX11Polygon::D2, SCREEN_X, SCREEN_Y);
	graphic.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(graphic.pTexture);


	//downPolygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X + 100)/2, SCREEN_Y/2);
	//down.CreateRenderTarget(SCREEN_X / 2, SCREEN_Y / 2, cl);
	//downPolygon->SetTexture(down.pTexture);

	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
		float calc = (i + 1);
		downSamp[i].polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) / calc, SCREEN_Y / calc);
		downSamp[i].graphic.CreateRenderTarget(SCREEN_X / calc, SCREEN_Y / calc, cl);
		downSamp[i].polygon->SetTexture(downSamp->graphic.pTexture);
	}

	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
		float calc = (i + 1);
		upSamp[BLUR_SAMPLE_NUM - 1 - i].polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X ) / calc, SCREEN_Y / calc);
		upSamp[BLUR_SAMPLE_NUM - 1 - i].graphic.CreateRenderTarget(SCREEN_X / calc, SCREEN_Y / calc, cl);
		upSamp[BLUR_SAMPLE_NUM - 1 - i].polygon->SetTexture(upSamp->graphic.pTexture);
	}


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

void Blur::Render() {
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	setBuffer.power.x = 0.020f;
	setBuffer.power.y = 0.020f;
	setBuffer.damy.x = setBuffer.damy.y = 0.0f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);

	D3D11_VIEWPORT defView;
	UINT vn = 1;

	// もしおかしかったらビューポート設定を見直す

	// ダウンサンプル
	downSamp[0].polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
		GetDX11DeviceContext()->ClearRenderTargetView(downSamp[i].graphic.pRenderTarget, cl);
		GetDX11DeviceContext()->OMSetRenderTargets(2, &downSamp[i].graphic.pRenderTarget, nullptr);

		// 一番最初でないなら一つ前のテクスチャをセット
		if (i != 0)downSamp[i].polygon->SetTexture(downSamp[i - 1].graphic.pTexture);

		downSamp[i].polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
		downSamp[i].polygon->SetPos(0.0f, 0.0f, 0.0f);
		downSamp[i].polygon->SetScale(1.0f, 1.0f);
		TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLUR);
		//GetDX11DeviceContext()->RSSetViewports(2,&setView);
		downSamp[i].polygon->Draw();
	}

	// アップサンプル
	upSamp[0].polygon->SetTexture(downSamp[BLUR_SAMPLE_NUM - 1].graphic.pTexture);
	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
		GetDX11DeviceContext()->ClearRenderTargetView(upSamp[i].graphic.pRenderTarget, cl);
		GetDX11DeviceContext()->OMSetRenderTargets(2, &upSamp[i].graphic.pRenderTarget, nullptr);
	
		// 一番最初でないなら一つ前のテクスチャをセット
		if (i != 0)upSamp[i].polygon->SetTexture(upSamp[i - 1].graphic.pTexture);
	
		upSamp[i].polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
		upSamp[i].polygon->SetPos(0.0f, 0.0f, 0.0f);
		upSamp[i].polygon->SetScale(1.0f, 1.0f);
		TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLUR);
		//GetDX11DeviceContext()->RSSetViewports(2, &setView);
	
		upSamp[i].polygon->Draw();
	}

	// レンダーターゲットを戻す
	GetDX11Graphics()->EnableZBuffer(false);
	polygon->SetTexture(upSamp[BLUR_SAMPLE_NUM - 1].graphic.pTexture);
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(0.0f, 0.0f, 0.0f);
	polygon->SetScale(1.0f, 1.0f);
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLUR);
	polygon->Draw();
}



// 時間がないのでコピペ、後で書き直す
void Blur::BlurTexture(GraphicTexture *pTex ) {
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	setBuffer.power.x = 0.022f;
	setBuffer.power.y = 0.022f;
	setBuffer.damy.x = setBuffer.damy.y = 0.0f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);

	// もしおかしかったらビューポート設定を見直す

	// ダウンサンプル
	downSamp[0].polygon->SetTexture(pTex->pTexture);
	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
	
		if(i != 0) GetDX11DeviceContext()->ClearRenderTargetView(downSamp[i].graphic.pRenderTarget, cl);
		GetDX11DeviceContext()->OMSetRenderTargets(2, &downSamp[i].graphic.pRenderTarget, nullptr);

		// 一番最初でないなら一つ前のテクスチャをセット
		if (i != 0)downSamp[i].polygon->SetTexture(downSamp[i - 1].graphic.pTexture);

		downSamp[i].polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
		downSamp[i].polygon->SetPos(0.0f, 0.0f, 0.0f);
		downSamp[i].polygon->SetScale(1.0f, 1.0f);
		TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLUR);
		//GetDX11DeviceContext()->RSSetViewports(2,&setView);
		downSamp[i].polygon->Draw();
	}

	// アップサンプル
	upSamp[0].polygon->SetTexture(downSamp[BLUR_SAMPLE_NUM - 1].graphic.pTexture);
	for (int i = 0; i < BLUR_SAMPLE_NUM; i++) {
		GetDX11DeviceContext()->ClearRenderTargetView(upSamp[i].graphic.pRenderTarget, cl);
		GetDX11DeviceContext()->OMSetRenderTargets(2, &upSamp[i].graphic.pRenderTarget, nullptr);

		// 一番最初でないなら一つ前のテクスチャをセット
		if (i != 0)upSamp[i].polygon->SetTexture(upSamp[i - 1].graphic.pTexture);

		upSamp[i].polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
		upSamp[i].polygon->SetPos(0.0f, 0.0f, 0.0f);
		upSamp[i].polygon->SetScale(1.0f, 1.0f);
		TextureShaderSelector::Get()->SelectShader(SHADER_TEX_BLUR);
		//GetDX11DeviceContext()->RSSetViewports(2, &setView);

		upSamp[i].polygon->Draw();
	}


	// ぼかした後になる
	*pTex = upSamp[BLUR_SAMPLE_NUM - 1].graphic;
	

	// レンダーターゲット戻す
	//GetDX11Graphics()->EnableZBuffer(false);
}