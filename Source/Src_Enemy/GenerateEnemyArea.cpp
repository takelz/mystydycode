
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

#define CSV_DATA_NUM			(2)		// �s�̃f�[�^�̐�

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
	auto object = loadData.begin();									// ��ƍs�Ȃ̂œ񎟌����X�g
	for (unsigned int num = 0; num < loadData.size(); num++) {		// loadData�̃T�C�Y�i�傫�����j
		auto line = object->begin();									// ���W���̃f�[�^���X�g�̍ŏ���line
		int objIntData[CSV_DATA_NUM];									// �f�[�^���i�[���Ă����z��

		for (unsigned int data = 0; data < object->size(); data++) { // �f�[�^���i�[�B�|�C���^�Ȃ̂ŃA�X�^���X�N��Y��Ȃ�
			objIntData[data] = *line;
			line++;	// �|�C���^�ړ�
		}
		// ��Ԗڂ͓G�̎�ށA��Ԗڂ͏o������
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

	// �C�e���[�^�ŕ\�̏ォ�珇�ԂɌ��Ă���
	// time����v���Ă����琶�����A�C�e���[�^������	
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
