#include"SceneManager.h"
#include"../GameScene.h"


void SceneManager::LoadScene(SceneNum sceneNum) {
	setScene = sceneNum;
}

void SceneManager::Repeat() {
	currentScene = SCENE_MAX;
}

void SceneManager::SceneTrance() {
	if (setScene == currentScene) return;	// �X�V���Ȃ������珈�����Ȃ�


	if (pScene != nullptr) {	// �Ȃɂ������Ă��������
		delete pScene;
		pScene = nullptr;
	}
	GameObjectList::Get()->Reset();	

	currentScene = setScene;

	switch (setScene) {
		case SCENE_MAIN:
			pScene = new GameScene();
			pScene->LoadObject();
			break;
		case SCENE_TITLE:
			pScene = new TitleScene();
			pScene->LoadObject();
			break;
		case SCENE_GAMEOVER:
			break;
			
	}



}
