/**
* @brief デバッグテキスト
* @author ryota.ssk
*/
#ifndef __DEBUG_TEXT_H__
#define __DEBUG_TEXT_H__

#include <DirectX/WinFont.h>
#include <string>
#include <vector>


#define DbgText DebugText::Instance()

/**
* @brief デバッグテキスト
*/
class DebugText
{
	//----------
	// 構造体、列挙型定義
	//----------
private:
	typedef std::vector<std::string> TextList;

	//----------
	// 関数
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
	// 変数
	//----------
private:
	static DebugText *m_pInstance;
private:
	WinFont		*m_pFont;	///< 表示フォント
	TextList	m_textList;	///< 表示テキストリスト
};

#endif // __DEBUG_TEXT_H__