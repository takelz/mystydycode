/**
 * @file WinFont.cpp
 * @brief winapiでフォント作成
 * @author ryota.ssk
 */
#include <DirectX/WinFont.h>
#include <DirectX/DX11Polygon.h>
#include <DirectX/DX11Texture.h>
#include <Debug/DebugUtility.h>

WinFont::WinFont(int size, int edge, int chace)
	: m_size(size)
	, m_edge(edge)
{
	m_texture.resize(chace);

	m_pPolygon = NEW DX11Polygon(DX11Polygon::D2, 
		static_cast<float>(m_size), static_cast<float>(m_size));
	m_pPolygon->SetAnchor(DX11Polygon::LeftTop);

	m_pos.x = 0;
	m_pos.y = 0;
}
WinFont::~WinFont()
{
	SAFE_DELETE(m_pPolygon);
	TextureList::iterator it = m_texture.begin();
	while (it != m_texture.end()) {
		TEXTURE->Release(it->pResource);
		++it;
	}
}

void WinFont::SetPos(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void WinFont::Draw(const char *text)
{
	DirectX::XMFLOAT2 pos = m_pos;

	// 一文字ずつ解析して表示
	UINT idx = 0;
	while (idx < strlen(text)) {
		Code code = GetCode(&text[idx]);
		// 改行
		if (code.code == '\n') {
			pos.x = m_pos.x;
			pos.y += m_size;
		}
		// 半角スペース
		else if(code.code == ' ') {
			pos.x += m_size / 2;
		}
		// 全角スペース
		else if (code.code == '　') {
			pos.x += m_size;
		}
		// タブ
		else if (code.code == '\t') {
			pos.x += m_size * 2;
		}
		// 文字表示
		else {
			Texture texture;
			int texIdx = FindTexture(code);
			if (texIdx != -1) {
				texture = m_texture[texIdx];
			}
			else {
				texture = CreateTexture(code);
				texIdx = static_cast<int>(m_texture.size() - 1);
			}
			if (texture.code != 0) {
				// テクスチャキャッシュ更新
				for (int i = texIdx; i > 0; --i) {
					m_texture[i] = m_texture[i - 1];
				}
				m_texture[0] = texture;
				// 描画
				m_pPolygon->SetPos(
					pos.x + texture.adjust.x,
					pos.y + texture.adjust.y, 0);
				m_pPolygon->SetScale(
					static_cast<float>(texture.size.cx) / m_size,
					static_cast<float>(texture.size.cy) / m_size);
				m_pPolygon->SetTexture(texture.pResource);
				m_pPolygon->Draw();
				pos.x += texture.size.cx;
			}
			else {
				// 文字が表示できなかった
				pos.x += m_size;
			}
		}

		idx += code.len;
	}
}

/**
 * @brief 現在のフォント情報で描画サイズを計算
 * @param[in] text 文字列
 * @return 描画サイズ
 */
SIZE WinFont::CalcSize(const char *text)
{
	SIZE size = { 0, static_cast<LONG>(m_size) };
	UINT idx = 0;
	LONG w = 0;
	while (idx < strlen(text)) {
		Code code = GetCode(&text[idx]);
		if (code.code == '\n') {
			size.cx = max(w, size.cx);
			size.cy += m_size;
			w = 0;
		}
		else {
			w += (m_size * code.len + 1) / 2;
		}
		idx += code.len;
	}
	size.cx = max(w, size.cx);
	return size;
}

/**
 * @brief 文字情報取得
 */
WinFont::Code WinFont::GetCode(const char *c)
{
	Code code;
	code.c = c;
	code.len = IsDBCSLeadByte(c[0]) ? 2 : 1;
	if (code.len == 2) {
		code.code = (unsigned char)c[0] << 8 | (unsigned char)c[1];
	}
	else {
		code.code = (unsigned char)c[0];
	}
	return code;
}

/**
 * @brief テクスチャ検索
 */
int WinFont::FindTexture(const Code &code)
{
	TextureList::iterator it = m_texture.begin();
	while (it != m_texture.end()) {
		if (it->code == code.code) {
			return static_cast<int>(it - m_texture.begin());
		}
		++it;
	}
	return -1;
}

/**
 * @brief テクスチャ作成
 */
WinFont::Texture WinFont::CreateTexture(const Code &code)
{
	Texture texture;
	const char *faceName = "MSゴシック";
	UINT penSize = m_edge;

	// デバイスコンテキスト作成
	HDC hDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);

	// フォント作成
	LOGFONTA lf = {};
	lf.lfHeight = m_size;
	lf.lfCharSet = SHIFTJIS_CHARSET;
	lf.lfWeight = 0;
	lf.lfQuality = ANTIALIASED_QUALITY;
	memcpy(lf.lfFaceName, faceName, strlen(faceName));
	HFONT hFont = CreateFontIndirectA(&lf);
	HFONT oldFont = (HFONT)SelectObject(memDC, hFont);

	// フォントサイズの計算
	TEXTMETRICA tm;
	GLYPHMETRICS gm;
	MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	GetTextMetricsA(memDC, &tm);
	GetGlyphOutlineA(memDC, code.code, GGO_METRICS, &gm, 0, 0, &mat);

	const LONG penHalfSize = penSize / 2;
	RECT charRegion = {
		gm.gmptGlyphOrigin.x - penHalfSize,
		tm.tmAscent - gm.gmptGlyphOrigin.y - penHalfSize,
		gm.gmptGlyphOrigin.x + static_cast<LONG>(gm.gmBlackBoxX) + penHalfSize,
		tm.tmAscent - gm.gmptGlyphOrigin.y + static_cast<LONG>(gm.gmBlackBoxY) + penHalfSize
	};
	texture.size.cx = gm.gmBlackBoxX + penSize;
	texture.size.cy = gm.gmBlackBoxY + penSize;
	texture.adjust.x = charRegion.left;
	texture.adjust.y = charRegion.top;

	// api書き込み先の領域を作成
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = texture.size.cx;
	bmpInfo.bmiHeader.biHeight = -texture.size.cy;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	BYTE *pBit = 0;
	HBITMAP hBitMap = CreateDIBSection(0, &bmpInfo, DIB_RGB_COLORS, (void**)&pBit, 0, 0);
	HBITMAP oldBMP = (HBITMAP)SelectObject(memDC, hBitMap);

	// 背景を青色で初期化
	RECT charWH = { 0, 0, texture.size.cx, texture.size.cy };
	HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
	FillRect(memDC, &charWH, bgBrush);
	DeleteObject(bgBrush);

	// 文字描画(パスは緑、塗りは赤
	HPEN   hPen = (HPEN)CreatePen(PS_SOLID, penSize, RGB(0, 255, 0));
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	HPEN   oldPen = (HPEN)SelectObject(memDC, hPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	SetBkMode(memDC, TRANSPARENT);
	BeginPath(memDC);
	TextOutA(memDC, -charRegion.left, -charRegion.top, code.c, code.len);
	EndPath(memDC);
	StrokeAndFillPath(memDC);

	// 作成したオブジェクト削除
	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldFont);
	SelectObject(memDC, oldBMP);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);


	// BMPデータをテクスチャデータに変換
	UINT texW = texture.size.cx;
	UINT texH = texture.size.cy;
	D3DCOLORVALUEU *pTex = NEW D3DCOLORVALUEU[texW * texH];
	const UINT PixelSize = 3;
	const UINT Align = 4;
	UINT BMPPitch = (texW * PixelSize + (Align - 1)) / Align * Align;

	for (UINT y = 0; y < texH; ++ y) {
		for (UINT x = 0; x < texW; ++ x) {
			int texIdx = texW * y + x;
			int bitIdx = BMPPitch * y + PixelSize * x;
			if (pBit[bitIdx + 0] == 255) {
				// 透過
				pTex[texIdx].a = 0;
			}
			else {
				pTex[texIdx].a = 255;
				if (pBit[bitIdx + 1] == 255 && m_edge != 0) {
					// 黒い縁取り
					pTex[texIdx].r = pTex[texIdx].g = pTex[texIdx].b = 0;
				}
				else if(pBit[bitIdx + 2] == 255){
					// 白い塗りつぶし
					pTex[texIdx].r = pTex[texIdx].g = pTex[texIdx].b = 255;
				}
				else {
					// 透過
					pTex[texIdx].a = 0;
				}
			}
		}
	}

	// テクスチャ作成
	HRESULT hr = E_FAIL;
	ID3D11Device *pDevice = GetDX11Device();
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = texW;
	texDesc.Height = texH;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = pTex;
	initData.SysMemPitch = texW * Align;

	ID3D11Texture2D *pTexture;
	hr = pDevice->CreateTexture2D(&texDesc, &initData, &pTexture);
	SAFE_DELETE(pTex);
	if (SUCCEEDED(hr)) {
		// シェーダリソース作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		hr = pDevice->CreateShaderResourceView(pTexture, &srvDesc, &texture.pResource);
		//TEXTURE->Entry(texture.pResource);
	}
	SAFE_RELEASE(pTexture);
	DeleteObject(hBitMap);
	DeleteDC(memDC);

	if (SUCCEEDED(hr)) {
		texture.code = code.code;
	}
	else {
		texture.code = 0;
	}

	return texture;
}