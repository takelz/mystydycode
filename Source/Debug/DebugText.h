/**
* @brief �f�o�b�O�e�L�X�g
* @author ryota.ssk
*/
#ifndef __DEBUG_TEXT_H__
#define __DEBUG_TEXT_H__

#include <DirectX/WinFont.h>
#include <string>
#include <vector>


#define DbgText DebugText::Instance()

/**
* @brief �f�o�b�O�e�L�X�g
*/
class DebugText
{
	//----------
	// �\���́A�񋓌^��`
	//----------
private:
	typedef std::vector<std::string> TextList;

	//----------
	// �֐�
	//----------
public:
	static DebugText* Instance();
	static void Create();
	static void Delete();
public:
	DebugText();
	~DebugText();

	void SetString(const char *format, ...);
	void Draw();

	//----------
	// �ϐ�
	//----------
private:
	static DebugText *m_pInstance;
private:
	WinFont		*m_pFont;	///< �\���t�H���g
	TextList	m_textList;	///< �\���e�L�X�g���X�g
};

#endif // __DEBUG_TEXT_H__