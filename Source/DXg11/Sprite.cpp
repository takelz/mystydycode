#include"Sprite.h"
#include"../DirectX/DirectXTex/WICTextureLoader.h"
#include<string>
#include<Shader/Shader.h>
#include"../Game.h"


using namespace DirectX;

// これらはコンパイル済みのシェーダーのこと
//const char *POLYGON_VERTEX_SHADER = "Assets/Shader/polygon_vs.cso";
//const char *POLYGON_PIXEL_SHADER = "Assets/Shader/polygon_ps.cso";



#define _DEFAULT_NUM_SPRITE_SIZE (1.0f)

Sprite::Sprite() : polygon(DX11Polygon::D2, _DEFAULT_NUM_SPRITE_SIZE, _DEFAULT_NUM_SPRITE_SIZE)
{
	
	dimension = D2;
	interpolation = POINT;

	// 後にVectorに変える
	size = XMFLOAT2(_DEFAULT_NUM_SPRITE_SIZE, _DEFAULT_NUM_SPRITE_SIZE);
	pos = Vector3(0, 0,0);
	scale = Vector3(1, 1,0);
	color = XMFLOAT4(1,1,1,1);

}

Sprite::~Sprite() {
	//TextureManager::Get()->Delete();
}


void Sprite::Load(const char *name) {
	polygon.SetTexture(name);
	
}

void Sprite::Render() {
	DX11Graphics *pGraphics = GetDX11Graphics();
	//pGraphics->EnableZBuffer(false);
	GetDX11Light()->SetEnable(false);
	GetDX11Light()->Update();
	
	
	polygon.SetPos(pos.x, pos.y, pos.z);
	polygon.SetScale(scale.x, scale.y);
	//polygon.SetColor(1.0f, 1.0f, 1.0f, 1.0f);	// あとで別仕様に
	polygon.Draw();	

	//pGraphics->EnableZBuffer(true);
	GetDX11Light()->SetEnable(true);
	//
	GetDX11Light()->Update();
}

void Sprite::SetDimension(DimensionKind d) {
	polygon.m_dimension = (DX11Polygon::DimensionKind)d;
}

void Sprite::SetInterpolation(InterpolationKind i) {
	polygon.SetInterpolation((DX11Polygon::InterpolationKind)i);
}


void Sprite::SetAnchor(AnchorKind a) {
	polygon.SetAnchor((DX11Polygon::AnchorKind)a);
}

void Sprite::SetColor(float r, float g, float b, float a) {
	polygon.SetColor(r, g, b, a);
}

