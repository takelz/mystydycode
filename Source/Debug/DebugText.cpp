#include "DebugText.h"
#include "DebugUtility.h"
#include <stdarg.h>

//----------
// 定数
//----------
const int FontSize = 23;			///< フォントサイズ

DebugText* DebugText::m_pInstance = NULL;

/**
 * @brief インスタンス取得
 * @return インスタンス
 */
DebugText* DebugText::Instance()
{
	return m_pInstance;
}
/**
 * @brief インスタンス生成
 */
void DebugText::Create()
{
	if (m_pInstance == NULL) {
		m_pInstance = NEW DebugText();
	}
}
/**
 * @brief インスタンス削除
 */
void DebugText::Delete()
{
	SAFE_DELETE(m_pInstance);
}

/**
 * @brief コンストラクタ
 */
DebugText::DebugText()
{
	m_pFont = NEW WinFont(FontSize, 1, 200);
}
/**
 * @brief デストラクタ
 */
DebugText::~DebugText()
{
	SAFE_DELETE(m_pFont);
}

/**
 * @brief 文字列登録
 * @param[in] format 文字フォーマット
 * @param[in] ... 可変長引数
 */
void DebugText::SetString(const char *format, ...)
{
	char buf[512];
	va_list args;
	va_start(args, format);
	vsprintf_s(buf, sizeof(buf), format, args);
	va_end(args);

	m_textList.push_back(buf);
}
/**
 * @brief 描画
 */
void DebugText::Draw()
{
	TextList::iterator it = m_textList.begin();
	DirectX::XMFLOAT2 pos(500, 20);
	while (it != m_textList.end())
	{
		m_pFont->SetPos(pos.x, pos.y);
		m_pFont->Draw(it->c_str());
		pos.y += FontSize;
		++it;
	}
	m_textList.clear();
}

// EOF