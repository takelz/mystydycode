#include"DepthBuffer.h"
#include"ShaderSelector.h"
#include"../DXg11/GameObjectBase.h"

using namespace DirectX;


void DepthBuffer::Create() {

	polygon = new DX11Polygon(DX11Polygon::D2, (SCREEN_X) , SCREEN_Y );
	float cl[4] = { 1.0f,1.0f,1.0f,1.0f };
	graphic.CreateRenderTarget(SCREEN_X, SCREEN_Y, cl);
	polygon->SetTexture(graphic.pTexture);


}

void DepthBuffer::Render() {
	// Šm”F—p
	polygon->SetAnchor(DX11Polygon::AnchorKind::LeftTop);
	polygon->SetPos(50.0f, 50.0f, 0.0f);
	polygon->SetScale(0.3f, 0.3f);
	polygon->Draw();
}