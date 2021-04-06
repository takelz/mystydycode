
#include"Barrier.h"
#include<DirectX/DrawObject.h>
#include"../Camera.h"
#include<Shader/PostDistortionObject.h>
#include<Shader/NormalRender.h>

using namespace DirectX;



// 座標0,0,0に置く

#define LIFE_TIME (20)

Barrier::Barrier() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/sphere.fbx");

	transform.scale = 250.0f;
	graphicTexture.Load("Assets/Textures/shader/hex.png");
	pTexture = graphicTexture.pTexture;
	randomGraphicTexture.Load("Assets/Textures/shader/random.jpg");
	pRandomTexture = randomGraphicTexture.pTexture;

	isOn = false;
	GameObjectList::Get()->AddDrawList(this);
}	

void Barrier::Update() {

}

void Barrier::LateDraw() {
	if (!isOn) {
		return;
	}
	if (RENDER_PATH == PATH_SHADOW_NUM) return;		// 影は描かない


	if (RENDER_PATH == 3) {

		// 通常描画
		ShaderSelector::Get()->SelectModelShader(SHADER_BARRIER);
		ShaderSelector::Get()->SetConstantBuffers()->
			PSSetConstantBuffers(1, 1, GetDX11Light()->GetBuffer());
		// カメラ
		ShaderSelector::Get()->SetConstantBuffers()->
			PSSetConstantBuffers(2, 1, Camera::Instance()->GetBuffer());

		ShaderSelector::Get()->SetConstantBuffers()->PSSetShaderResources(1, 1, &pTexture);
		ShaderSelector::Get()->SetConstantBuffers()->PSSetShaderResources(2, 1, &pRandomTexture);
		GetModel()->SetTextureWrap(true);		// シェーダーのためにモデルのテクスチャ繰り返し有効
		GetModel()->Draw(transform);		
	}

	if (RENDER_PATH == 5) {
		GetModel()->Draw(transform);
	}
}
