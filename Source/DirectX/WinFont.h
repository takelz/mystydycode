/**
 * @file WinFont.h
 * @brief winapi�Ńt�H���g�쐬
 * @author ryota.ssk
 */
#ifndef __WIN_FONT_H__
#define __WIN_FONT_H__

#include "DX11Util.h"
#include <vector>

class DX11Polygon;

class WinFont
{
private:
	struct Texture {
		UINT code;		// �����R�[�h
		POINT adjust;	// �����ʒu
		SIZE size;		// �����T�C�Y
		ID3D11ShaderResourceView *pResource;
	};
	typedef std::vector<Texture> TextureList;

	struct Code {
		const char *c;	// ����
		BYTE len;		// �o�C�g��
		UINT code;		// �����R�[�h
	};

public:
	/**
	 * @param[in] size �����T�C�Y
	 * @param[in] edge ���T�C�Y
	 * @param[in] chace �e�N�X�`���L���b�V��
	 */
	WinFont(int size, int edge = 1, int chace = 100);
	~WinFont();

	void SetPos(float x, float y);
	void Draw(const char *text);

	/**
	 * @brief ���݂̃t�H���g���ŕ`��T�C�Y���v�Z
	 * @param[in] text ������
	 * @return �`��T�C�Y
	 */
	SIZE CalcSize(const char *text);

private:
	Code GetCode(const char *c);
	int FindTexture(const Code &code);
	Texture CreateTexture(const Code &code);

private:
	UINT m_size;
	UINT m_edge;
	TextureList m_texture;

	DX11Polygon *m_pPolygon;
	DirectX::XMFLOAT2 m_pos;
};

#endif // __WIN_FONT_H__