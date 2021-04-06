
#include"GenerateAreaManager.h"
#include"../TimeProgress.h"
#include"../StageManager.h"
#include"../Src_Scene/WaveUI.h"

#define POINT_A Vector3(3000.0f,	0.0f,	0.0f)
#define POINT_B Vector3(2500.0f,	0.0f,	2500.0f)
#define POINT_C Vector3(0.0f,		0.0f,	3000.0f)
#define POINT_D Vector3(-2500.0f,	0.0f,	2500.0f)
#define POINT_E Vector3(-3000.0f,	0.0f,	0.0f)
#define POINT_F Vector3(2500.0f,	0.0f,	-2500.0f)
#define POINT_G Vector3(0.0f,		0.0f,	-3000.0f)
#define POINT_H Vector3(-2500.0f,	0.0f,	-2500.0f)


#define MAX_WAVE (3)

using namespace DirectX;

GenerateAreaManager::GenerateAreaManager() {
	currentWave = 0;
	endGenerate = false;
	state = STATE_MAX;
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		generateArea[i] = nullptr;;
	}
}

void GenerateAreaManager::Update() {
	switch (state)
	{
	case GenerateAreaManager::WAVE:
		EndWave();
		break;
	case GenerateAreaManager::INTERVAL:
		Interval();
		break;
	case GenerateAreaManager::CLEAR:
		StageManager::Get()->GameClear();
		break;
	default:
		break;
	}
}

void GenerateAreaManager::Wave01() {
	SetupArea();
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		//generateArea[i]->LoadData("Assets/stages/stage.csv");

	}

	
	generateArea[0]->LoadData("Assets/stages/wave01/01_a.csv");
	generateArea[1]->LoadData("Assets/stages/wave01/01_b.csv");
	generateArea[2]->LoadData("Assets/stages/wave01/01_c.csv");
	generateArea[3]->LoadData("Assets/stages/wave01/01_d.csv");
	generateArea[4]->LoadData("Assets/stages/wave01/01_e.csv");
	generateArea[5]->LoadData("Assets/stages/wave01/01_f.csv");
	generateArea[6]->LoadData("Assets/stages/wave01/01_g.csv");
	generateArea[7]->LoadData("Assets/stages/wave01/01_h.csv");
	

}

void GenerateAreaManager::Wave02() {
	SetupArea();
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		//generateArea[i]->LoadData("Assets/stages/stage2.csv");

	}
	
	generateArea[0]->LoadData("Assets/stages/wave02/01_a.csv");
	generateArea[1]->LoadData("Assets/stages/wave02/01_b.csv");
	generateArea[2]->LoadData("Assets/stages/wave02/01_c.csv");
	generateArea[3]->LoadData("Assets/stages/wave02/01_d.csv");
	generateArea[4]->LoadData("Assets/stages/wave02/01_e.csv");
	generateArea[5]->LoadData("Assets/stages/wave02/01_f.csv");
	generateArea[6]->LoadData("Assets/stages/wave02/01_g.csv");
	generateArea[7]->LoadData("Assets/stages/wave02/01_h.csv");
	
}

void GenerateAreaManager::Wave03() {
	SetupArea();
	//for (int i = 0; i < MAX_AREA_ARRAY; i++) {
	//	generateArea[i]->LoadData("Assets/stages/stag3.csv");
	//
	//}
	
	generateArea[0]->LoadData("Assets/stages/wave03/01_a.csv");
	generateArea[1]->LoadData("Assets/stages/wave03/01_b.csv");
	generateArea[2]->LoadData("Assets/stages/wave03/01_c.csv");
	generateArea[3]->LoadData("Assets/stages/wave03/01_d.csv");
	generateArea[4]->LoadData("Assets/stages/wave03/01_e.csv");
	generateArea[5]->LoadData("Assets/stages/wave03/01_f.csv");
	generateArea[6]->LoadData("Assets/stages/wave03/01_g.csv");
	generateArea[7]->LoadData("Assets/stages/wave03/01_h.csv");
	

}

void GenerateAreaManager::Wave04() {

}

void GenerateAreaManager::Wave05() {

}

void GenerateAreaManager::StartStage() {
	Wave01();
	currentWave = 1;
}


void GenerateAreaManager::NextWave() {
	switch (currentWave)
	{
	case 0:
		break;
	case 1:	Wave01();
		break;
	case 2:	Wave02();
		GameObjectList::Get()->Generate(new WaveUI("Assets/Textures/wave/wave2.png"));
		break;
	case 3:	Wave03();
		GameObjectList::Get()->Generate(new WaveUI("Assets/Textures/wave/wave3.png"));
		break;

	default:
		break;
	}

}

void GenerateAreaManager::EndWave() {
	if (endGenerate) return;
	bool isEnd = true;
	// 後で治す
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		isEnd = generateArea[i]->GetEnd();
	}


	// ここにステージ上の敵の数を見てZEROなら次へ
	if (0 < EnemyBase::GetNum()){
		isEnd = false;
	}

	if (isEnd) endGenerate = true;
	else return;
	// 終わってないならリターン

	state = INTERVAL;
	currentWave++;
	// 削除してまた次に生成
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		generateArea[i]->Destroy();
		generateArea[i] = nullptr;
	}
}

void GenerateAreaManager::Interval() {
	NextWave();
	if (currentWave > MAX_WAVE) {
		state = CLEAR;
	}
}


void GenerateAreaManager::SetupArea() {
	for (int i = 0; i < MAX_AREA_ARRAY; i++) {
		GenerateEnemyArea *obj = new GenerateEnemyArea();
		GameObjectList::Get()->Generate(obj);
		generateArea[i] = obj;
	}
	generateArea[0]->transform.position = POINT_A;
	generateArea[1]->transform.position = POINT_B;
	generateArea[2]->transform.position = POINT_C;
	generateArea[3]->transform.position = POINT_D;
	generateArea[4]->transform.position = POINT_E;
	generateArea[5]->transform.position = POINT_F;
	generateArea[6]->transform.position = POINT_G;
	generateArea[7]->transform.position = POINT_H;
	
	Time::Reset();

	state = WAVE;
	endGenerate = false;
}



