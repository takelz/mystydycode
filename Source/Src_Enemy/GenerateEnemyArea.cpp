
#include"GenerateEnemyArea.h"
#include"StandardEnemy.h"
#include"ShotEnemy.h"
#include"AttackEnemy.h"
#include<TimeProgress.h>
#define GENERATE_HEIGHT			(DEFAULT_FALL_POS)

#define AREA_SIZE_X				(200)
#define AREA_SIZE_Z				(200)
#define AREA_HEIGHT				(10)

#define GENERATE_COUNT_FRAME	(300)

#define CSV_DATA_NUM			(2)		// 行のデータの数

using namespace DirectX;

GenerateEnemyArea::GenerateEnemyArea() {
	//transform.position = Vector3(50, 0, 1000);
	generateCnt = GENERATE_COUNT_FRAME;
}

void GenerateEnemyArea::Update() {
	GenerateEnemy();
}

void GenerateEnemyArea::Draw() {

	debug::DrawShapeArg arg;
	arg.shape = debug::DrawShapeArg::ShapeType::ST_BOX;
	arg.pose = debug::DrawShapeArg::PoseType::PT_ROTATION;
	arg.center = debug::DrawShapeArg::CenterType::CT_ORIGIN;

	XMFLOAT3 center(0, 0, 0);
	XMMATRIX move = XMMatrixTranslationFromVector(XMLoadFloat3(&center));
	arg.uPose.mat = XMMatrixMultiply(move, XMLoadFloat4x4(&transform.GetMatrix()));
	arg.uSize.vec = XMFLOAT3(AREA_SIZE_X, AREA_HEIGHT, AREA_SIZE_Z);

	DrawShape(arg, XMFLOAT4(0, 1, 0, 1));
}

void GenerateEnemyArea::LoadData(const char *fileName) {
	loadData = csv.Load(fileName, CSV_DATA_NUM);
	auto object = loadData.begin();									// 列と行なので二次元リスト
	for (unsigned int num = 0; num < loadData.size(); num++) {		// loadDataのサイズ（大きい方）
		auto line = object->begin();									// 座標等のデータリストの最初にline
		int objIntData[CSV_DATA_NUM];									// データを格納していく配列

		for (unsigned int data = 0; data < object->size(); data++) { // データを格納。ポインタなのでアスタリスクを忘れない
			objIntData[data] = *line;
			line++;	// ポインタ移動
		}
		// 一番目は敵の種類、二番目は出現時間
		FileData dat;
		dat.type = objIntData[0];
		dat.time = objIntData[1];

		generateData.emplace_back(dat);

		object++;
	}
	itrGenerate = generateData.begin();
}

void GenerateEnemyArea::GenerateEnemy() {
	if (itrGenerate == generateData.end()) {
		end = true;
		return;
	}

	// イテレータで表の上から順番に見ていく
	// timeが一致していたら生成し、イテレータを次へ	
	if (Time::GetCount() == itrGenerate->time) {
		switch (itrGenerate->type)
		{
		case 0:
		{
			StandardEnemy *obj = new StandardEnemy();
			GameObjectList::Get()->Generate(obj);
			obj->transform.position = transform.position;
			obj->transform.position.y = GENERATE_HEIGHT;
		}
		break;
		case 1:
		{
			ShotEnemy *obj = new ShotEnemy();
			GameObjectList::Get()->Generate(obj);
			obj->transform.position = transform.position;
			obj->transform.position.y = GENERATE_HEIGHT;
	
		}
		break;
		case 2:
		{
			AttackEnemy *obj = new AttackEnemy();
			GameObjectList::Get()->Generate(obj);
			obj->transform.position = transform.position;
			obj->transform.position.y = GENERATE_HEIGHT;
			
		}
		break;
		default:
			break;
		}



		itrGenerate++;
	}



	/*
	generateCnt--;
	if (generateCnt < 0) {
		generateCnt = GENERATE_COUNT_FRAME;
		
		StandardEnemy *obj = new StandardEnemy();
		GameObjectList::Get()->Generate(obj);
		obj->transform.position = transform.position;
		obj->transform.position.y = GENERATE_HEIGHT;

		
	}
	*/
}
