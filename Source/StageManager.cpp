
#include"StageManager.h"
#include"DXg11/SceneManager.h"
#include<DirectX/DrawObject.h>
#include"Fade.h"
#include"Src_Scene/GameClear.h"
#include"Src_Scene/GameOver.h"


using namespace DirectX;


void StageManager::GameOver() {

	// ステージ削除
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
	// シーン繰り返し
	SceneManager::Get()->Repeat();
	
}

void StageManager::EndScene() {

}

void StageManager::Select() {
	if (!generateManager) MessageBox(NULL, "generateManagerの値がnullです。\nシーンヘッダーを確認してください", "error", MB_OK);
	generateManager->StartStage();
}