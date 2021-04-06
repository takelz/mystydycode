
#include"StageManager.h"
#include"DXg11/SceneManager.h"
#include<DirectX/DrawObject.h>
#include"Fade.h"
#include"Src_Scene/GameClear.h"
#include"Src_Scene/GameOver.h"


using namespace DirectX;


void StageManager::GameOver() {

	// �X�e�[�W�폜
	if (!gameover) {
		class GameOver *obj = new class GameOver();
		GameObjectList::Get()->Generate(obj);
		gameover = true;
	}


}

void StageManager::GameClear(){
	if (!gameclear) {
		class GameClear *obj = new class GameClear();
		GameObjectList::Get()->Generate(obj);
		gameclear = true;
	}
}

void StageManager::RestartStage() {
	// �V�[���J��Ԃ�
	SceneManager::Get()->Repeat();
	
}

void StageManager::EndScene() {

}

void StageManager::Select() {
	if (!generateManager) MessageBox(NULL, "generateManager�̒l��null�ł��B\n�V�[���w�b�_�[���m�F���Ă�������", "error", MB_OK);
	generateManager->StartStage();
}