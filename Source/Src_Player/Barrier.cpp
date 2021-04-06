
#include"Barrier.h"
#include<DirectX/DrawObject.h>
#include"../Camera.h"
#include<Shader/PostDistortionObject.h>
#include<Shader/NormalRender.h>

using namespace DirectX;



// ���W0,0,0�ɒu��

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
	if (RENDER_PATH == PATH_SHADOW_NUM) return;		// �e�͕`���Ȃ�


	if (RENDER_PATH == 3) {

		// �ʏ�`��
		ShaderSelector::Get()->SelectModelShader(SHADER_BARRIER);
		ShaderSelector::Get()->SetConstantBuffers()->
			PSSetConstantBuffers(1, 1, GetDX11Light()->GetBuffer());
		// �J����
		ShaderSelector::Get()->SetConstantBuffers()->
			PSSetConstantBuffers(2, 1, Camera::Instance()->GetBuffer());

		ShaderSelector::Get()->SetConstantBuffers()->PSSetShaderResources(1, 1, &pTexture);
		ShaderSelector::Get()->SetConstantBuffers()->PSSetShaderResources(2, 1, &pRandomTexture);
		GetModel()->SetTextureWrap(true);		// �V�F�[�_�[�̂��߂Ƀ��f���̃e�N�X�`���J��Ԃ��L��
		GetModel()->Draw(transform);		
	}

	if (RENDER_PATH == 5) {
		GetModel()->Draw(transform);
	}
}
