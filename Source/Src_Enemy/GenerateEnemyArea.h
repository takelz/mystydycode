#pragma once
#include<DXg11/GameObjectList.h>

#include<FileCSV.h>
#include"EnemyBase.h"

class GenerateEnemyArea :public GameObject{
public:
	GenerateEnemyArea();
	~GenerateEnemyArea() {};

	struct FileData {
		int type;		// �o��������G�̎��
		int time;		// ����
	};

	void Update();
	void Draw();

	void LoadData(const char* fileName);
	bool GetEnd() { return end; }

	int uniqueNum;		// ���ʔԍ��A�g��Ȃ�����
	char* readFileName;	// �ǂݍ��܂���t�@�C���̖��O


private:
	void GenerateEnemy();
	
	FileCSV csv;
	int generateCnt;
	int nextGenerate;		// generateData��Time�̕������w��
	bool end;

	// xz�̂�
	DirectX::Vector3 size;

	std::list<std::list<int>> loadData;				// �ǂݍ��ݗp�f�[�^
	std::list<FileData> generateData;				// �ǂݍ��񂾃f�[�^��ۑ�
	std::list<FileData>::iterator itrGenerate;		// �C�e���[�^


};

