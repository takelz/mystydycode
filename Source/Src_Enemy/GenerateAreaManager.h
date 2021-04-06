#pragma once
#include<DXg11/GameObjectList.h>
#include"../Src_Player/ProtectedObject.h"
#include"GenerateEnemyArea.h"

#define MAX_AREA_ARRAY (8)


class GenerateAreaManager :public GameObject {
public:
	GenerateAreaManager();
	~GenerateAreaManager() {};

	void Update();

	void StartStage();
private:
	void Wave01();
	void Wave02();
	void Wave03();
	void Wave04();
	void Wave05();


	void NextWave();
	void EndWave();
	void Interval();
	void SetupArea();

	enum STATE{
		WAVE,
		INTERVAL,
		CLEAR,
		STATE_MAX,
	};

	int currentWave;
	bool endGenerate;	// ‘S‚Ä‚Ì¶¬‚ªI‚í‚Á‚½
	STATE state;
	GenerateEnemyArea *generateArea[MAX_AREA_ARRAY];
};

