#include"ShadowWrite.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"

using namespace DirectX;


void ShadowWrite::Create(UINT width, UINT height) {

	polygon = new DX11Polygon(DX11Polygon::D2, 1200 * 4.5f, 750 * 4.5f);
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	graphic.CreateRenderTarget(1200, 750, cl);
	polygon->SetTexture(graphic.pTexture);


}

void ShadowWrite::Render() {
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(50.0f, 50.0f, 0.0f);
	polygon->SetScale(0.3f, 0.3f);
	polygon->Draw();
}