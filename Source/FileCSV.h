//	CSV�t�@�C���֌W
// 
//	2018/09/29	�쐬
//	int�^�̕ϐ��ɑΉ�
//  2018/11/23
//	float�^�ɕύX
//  2020/01/14
//	int�^�ɖ߂���

#ifndef __FILECSV__
#define __FILECSV__


#include<list>



class FileCSV {
public:
	// CSV�t�@�C���̃��[�h�B�f�[�^�̍s�Ɗi�[���������X�g��n��
	// ���X�g�̓��X�g�̒��̃��X�g<int>��p�ӂ���B[list<std::list<int>>]
	// int�^�̂ݑΉ��B
	std::list<std::list<int>> Load(const char *fileName, int lineNum);

	FileCSV() {};	
	~FileCSV() {};

private:

};

#endif // !__STAGEGENERATE__
