#include"StartGame.h"
#include<DXg11/SceneManager.h>


StartGame::StartGame() {


}

void StartGame::Update() {
	Push();
}

void StartGame::Draw() {


}

void StartGame::Push(){
	if (Input::GetKey(DIK_RETURN, INPUT::TRIGGER) || DIGetPadTrigger(PAD_START)) {
		pushed = true;
		Fade::StartFade();
	}
	// �Â����肫���ăt�F�[�h�C�����[�h�ɐ؂�ւ������
	if (Fade::Get() == Fade::FADE_IN) {
		if (!pushed) return;
		SceneManager::Get()->LoadScene(SCENE_MAIN);
	}
}
