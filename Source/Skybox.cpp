#include "Skybox.h"
#include "Camera.h"

#define OFFSET_POS_Y (-100.0f)

const char *PATH_SKY_MODEL = "Assets/sky/Skydome.FBX";

// gameobjectList�̈�ԍŏ��Ɉʒu�������������H

Skybox::Skybox() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load(PATH_SKY_MODEL);
	DontDestroy();
	transform.scale = 40.0f;

}

Skybox::~Skybox() {

}

void Skybox::Update() {
	
}

void Skybox::Draw() {

		// ���C�g�̉e�����󂯂Ȃ�

	GetDX11Light()->SetEnable(false);
	GetDX11Light()->Update();
	transform.position = Camera::Instance()->paramPosition;
	transform.position.y = OFFSET_POS_Y;
	GetModel()->Draw(transform);
	GetDX11Light()->SetEnable(true);
	GetDX11Light()->Update();
	
}