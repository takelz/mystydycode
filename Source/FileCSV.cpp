//
//

#include"FileCSV.h"
#include<fstream>
#include<sstream>
#include<string>
#include<Windows.h>

using namespace std;

list<list<int>> FileCSV::Load(const char *fileName, int lineNum) {
	list<list<int>> setList;// 返す値
	string lineStr;			// 一行分
	string str;				// 一つのブロック分
	istringstream iString;	// 値を使うため

	ifstream file(fileName);// 読み込み


	if (file.fail()) {	// エラーチェック
		MessageBox(NULL, "ステージファイル読み込みに失敗", "error", MB_OK);
	}

	int y = 0;	// 何行目か
	// 改行で区切られた間をひとまとめに読み込む
	while (getline(file, lineStr, '\n')) {
		istringstream isLineStr(lineStr);
		int x = 0;				// 何列目か
		list<int> lineList;

		while (getline(isLineStr, str, ',')) {	
			if (lineNum <= x) {
				MessageBox(NULL, "CSVファイル読み込み時に指定した \n 列の値が間違っています。", "CSV読み込みエラー",MB_OK);
				//return -1;
			}
			int inNum;

			iString = istringstream(str);

			iString >> inNum;
			
			lineList.push_back(inNum);
		}
		y++;
		setList.push_back(lineList);
	}
	file.close();

	return setList;	// 行数を返す

}