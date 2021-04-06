#ifndef __SHADER_H__
#define __SHADER_H__

#include <DirectX/DX11Util.h>
#include "GraphicTexture.h"

/// @brief 頂点レイアウト
enum InputLayoutKind
{
	VTX_COL,
	VTX_POLYGON,
	SKIN,
	EXPLOSION,
	MAX
};

//-----
// 各種頂点情報
//-----
struct VertexCOL
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};
struct VertexPOLYGON
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};
struct VertexSKIN
{
};

HRESULT CreateVertexShader(LPCTSTR fileName, InputLayoutKind kind, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout);
HRESULT CreatePixelShader(LPCTSTR fileName, ID3D11PixelShader** ppPixelShader);

#endif // __SHADER_H__