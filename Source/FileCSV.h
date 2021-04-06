//	CSVファイル関係
// 
//	2018/09/29	作成
//	int型の変数に対応
//  2018/11/23
//	float型に変更
//  2020/01/14
//	int型に戻した

#ifndef __FILECSV__
#define __FILECSV__


#include<list>



class FileCSV {
public:
	// CSVファイルのロード。データの行と格納したいリストを渡す
	// リストはリストの中のリスト<int>を用意する。[list<std::list<int>>]
	// int型のみ対応。
	std::list<std::list<int>> Load(const char *fileName, int lineNum);

	FileCSV() {};	
	~FileCSV() {};

private:

};

#endif // !__STAGEGENERATE__
