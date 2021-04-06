/**
 * @file WinFont.h
 * @brief winapiでフォント作成
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
		UINT code;		// 文字コード
		POINT adjust;	// 調整位置
		SIZE size;		// 文字サイズ
		ID3D11ShaderResourceView *pResource;
	};
	typedef std::vector<Texture> TextureList;

	struct Code {
		const char *c;	// 文字
		BYTE len;		// バイト長
		UINT code;		// 文字コード
	};

public:
	/**
	 * @param[in] size 文字サイズ
	 * @param[in] edge 縁サイズ
	 * @param[in] chace テクスチャキャッシュ
	 */
	WinFont(int size, int edge = 1, int chace = 100);
	~WinFont();

	void SetPos(float x, float y);
	void Draw(const char *text);

	/**
	 * @brief 現在のフォント情報で描画サイズを計算
	 * @param[in] text 文字列
	 * @return 描画サイズ
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