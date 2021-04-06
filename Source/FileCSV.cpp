//
//

#include"FileCSV.h"
#include<fstream>
#include<sstream>
#include<string>
#include<Windows.h>

using namespace std;

list<list<int>> FileCSV::Load(const char *fileName, int lineNum) {
	list<list<int>> setList;// �Ԃ��l
	string lineStr;			// ��s��
	string str;				// ��̃u���b�N��
	istringstream iString;	// �l���g������

	ifstream file(fileName);// �ǂݍ���


	if (file.fail()) {	// �G���[�`�F�b�N
		MessageBox(NULL, "�X�e�[�W�t�@�C���ǂݍ��݂Ɏ��s", "error", MB_OK);
	}

	int y = 0;	// ���s�ڂ�
	// ���s�ŋ�؂�ꂽ�Ԃ��ЂƂ܂Ƃ߂ɓǂݍ���
	while (getline(file, lineStr, '\n')) {
		istringstream isLineStr(lineStr);
		int x = 0;				// ����ڂ�
		list<int> lineList;

		while (getline(isLineStr, str, ',')) {	
			if (lineNum <= x) {
				MessageBox(NULL, "CSV�t�@�C���ǂݍ��ݎ��Ɏw�肵�� \n ��̒l���Ԉ���Ă��܂��B", "CSV�ǂݍ��݃G���[",MB_OK);
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

	return setList;	// �s����Ԃ�

}