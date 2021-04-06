#pragma once

#include"DXg11/GameObjectList.h"
#include"DXg11/SceneManager.h"
#include"Skybox.h"

#include"Src_Player/Player.h"
#include"Src_Player/ProtectedObject.h"
#include"Src_Enemy/StandardEnemy.h"
#include"Src_Enemy/GenerateEnemyArea.h"
#include"Src_Enemy/GenerateAreaManager.h"
#include"Src_Player/MarketCenter.h"
#include"Land.h"
#include"Number.h"
#include"StageManager.h"
#include"TimeProgress.h"
#include"Src_Scene/TitleCenter.h"
#include"Src_Scene/PoseMenu.h"
#include"StageWall.h"
#include"Camera.h"

class GameScene :public SceneBase {
public:
	GameScene() {};
	~GameScene() {};

	void LoadObject() {
		Camera::Instance()->notUpdate = false;	// �^�C�g���p�ɕύX���Ă����̂�߂�
		GetDX11Light()->SetEyePos(XMFLOAT3(1000.0f, 2500.0f, 0.0f));
	

		GetDX11Light()->SetDirection(XMFLOAT3(0.0f, 1.0f, 0.0f));
		GetDX11Light()->SetEnable(true);

		// �����l���Z�b�g
		UnitNormal::bulletAddPower = 0;
		UnitNormal::lifeAddPower = 0;

		// �V�[��
		StageManager::Get()->ResetState();

		// �������畁�ʂ̃I�u�W�F�N�g����
		GameObjectList::Get()->Generate(new Skybox());
		GameObjectList::Get()->Generate(new Land());

		GameObjectList::Get()->Generate(new Time());

		
		GameObjectList::Get()->Generate(new StageWall());
		
		// �v���C���[�֘A
		MarketCenter *mkt = (MarketCenter*)GameObjectList::Get()->Generate(new MarketCenter());

		Player *ply = (Player*)GameObjectList::Get()->Generate(new Player());
		ply->SetMarketCenter(mkt);
		
		
		// �X�e�[�W�֘A
		GenerateAreaManager *gam = (GenerateAreaManager*)GameObjectList::Get()->Generate(new GenerateAreaManager());

		StageManager::Get()->SetGenerateEnemyArea(gam);
		StageManager::Get()->Select();

		ProtectedObject *prt = new ProtectedObject();
		GameObjectList::Get()->Generate(prt);
		mkt->SetProtect(prt);

		GameObjectList::Get()->Generate(new PoseMenu());
	}

};

class TitleScene :public SceneBase {
public:
	TitleScene() {};
	~TitleScene() {};

	void LoadObject() {
		GetDX11Light()->SetEyePos(XMFLOAT3(100.0f, 500.0f, 0.0f));
		GetDX11Light()->SetDirection(XMFLOAT3(0.0f, 1.0f, 0.0f));
		GetDX11Light()->SetEnable(true);
		GameObjectList::Get()->Generate(new Skybox());
		GameObjectList::Get()->Generate(new Land());
		GameObjectList::Get()->Generate(new TitleCenter());
		GameObjectList::Get()->Generate(new StageWall());
		GameObjectList::Get()->Generate(new ProtectedObject());


	}

};

