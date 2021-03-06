#include"SceneManager.h"
#include"../GameScene.h"


void SceneManager::LoadScene(SceneNum sceneNum) {
	setScene = sceneNum;
}

void SceneManager::Repeat() {
	currentScene = SCENE_MAX;
}

void SceneManager::SceneTrance() {
	if (setScene == currentScene) return;	// 更新がなかったら処理しない


	if (pScene != nullptr) {	// なにか入っていたら消す
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
