#include "DebugText.h"
#include "DebugUtility.h"
#include <stdarg.h>

//----------
// �萔
//----------
const int FontSize = 23;			///< �t�H���g�T�C�Y

DebugText* DebugText::m_pInstance = NULL;

/**
 * @brief �C���X�^���X�擾
 * @return �C���X�^���X
 */
DebugText* DebugText::Instance()
{
	return m_pInstance;
}
/**
 * @brief �C���X�^���X����
 */
void DebugText::Create()
{
	if (m_pInstance == NULL) {
		m_pInstance = NEW DebugText();
	}
}
/**
 * @brief �C���X�^���X�폜
 */
void DebugText::Delete()
{
	SAFE_DELETE(m_pInstance);
}

/**
 * @brief �R���X�g���N�^
 */
DebugText::DebugText()
{
	m_pFont = NEW WinFont(FontSize, 1, 200);
}
/**
 * @brief �f�X�g���N�^
 */
DebugText::~DebugText()
{
	SAFE_DELETE(m_pFont);
}

/**
 * @brief ������o�^
 * @param[in] format �����t�H�[�}�b�g
 * @param[in] ... �ϒ�����
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
 * @brief �`��
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