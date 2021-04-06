#pragma once

#include<DXg11/GameObjectList.h>
#include"Src_Enemy/GenerateEnemyArea.h"
#include"Src_Enemy/GenerateAreaManager.h"


class StageManager {
public:
	StageManager() { generateManager = nullptr; };
	StageManager(const StageManager&) = delete;
	StageManager& operator=(const StageManager&) = delete;
	StageManager(StageManager&&) = delete;
	StageManager& operator=(StageManager&&) = delete;


	static StageManager* Get() {
		static StageManager instance;
		return &instance;
	}

	void GameOver();
	void GameClear();
	// ���݂̃X�e�[�W����蒼��
	void RestartStage();
	// �I��
	void EndScene();

	void Select();

	void SetGenerateEnemyArea(GenerateAreaManager* obj) { generateManager = obj; }
	void ResetState() { gameover = false; gameclear = false; }
private:
	GenerateAreaManager * generateManager;
	bool gameover;
	bool gameclear;

};



