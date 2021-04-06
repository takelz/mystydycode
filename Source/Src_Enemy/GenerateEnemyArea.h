#pragma once
#include<DXg11/GameObjectList.h>

#include<FileCSV.h>
#include"EnemyBase.h"

class GenerateEnemyArea :public GameObject{
public:
	GenerateEnemyArea();
	~GenerateEnemyArea() {};

	struct FileData {
		int type;		// 出現させる敵の種類
		int time;		// 時間
	};

	void Update();
	void Draw();

	void LoadData(const char* fileName);
	bool GetEnd() { return end; }

	int uniqueNum;		// 識別番号、使わないかも
	char* readFileName;	// 読み込ませるファイルの名前


private:
	void GenerateEnemy();
	
	FileCSV csv;
	int generateCnt;
	int nextGenerate;		// generateDataのTimeの部分を指す
	bool end;

	// xzのみ
	DirectX::Vector3 size;

	std::list<std::list<int>> loadData;				// 読み込み用データ
	std::list<FileData> generateData;				// 読み込んだデータを保存
	std::list<FileData>::iterator itrGenerate;		// イテレータ


};

