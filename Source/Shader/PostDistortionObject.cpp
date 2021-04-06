#include"PostDistortionObject.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"
#include"NormalRender.h"
#include"../Camera.h"

using namespace DirectX;


void PostDistortionObject::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	distNormal.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(distNormal.pTexture);
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

}

void PostDistortionObject::Render() {
	setBuffer.cameraDir.x = Camera::Instance()->GetLookVector().x;
	setBuffer.cameraDir.y = Camera::Instance()->GetLookVector().y;
	setBuffer.cameraDir.z = Camera::Instance()->GetLookVector().z;
	setBuffer.cameraDir.w = 0.0f;
	GetDX11DeviceContext()->UpdateSubresource(constBuffer, 0, NULL, &setBuffer, 0, 0);
	GetDX11DeviceContext()->PSSetConstantBuffers(0, 1, &constBuffer);

	// メインのレンダーにも描き込む
	//GetDX11DeviceContext()->OMSetRenderTargets(1, &NormalRender::Get()->graphic.pRenderTarget, nullptr);
	GetDX11Graphics()->EnableZBuffer(false); 
	TextureShaderSelector::Get()->SelectShader(SHADER_TEX_DISTORTION);
	GetDX11DeviceContext()->PSSetShaderResources(1, 1, &distNormal.pTexture);
	polygon->SetTexture(NormalRender::Get()->graphic.pTexture);
	polygon->SetScale(1.0f, 1.0f);
	polygon->Draw();


}
