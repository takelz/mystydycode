#ifndef __SHADER_H__
#define __SHADER_H__

#include <DirectX/DX11Util.h>
#include "GraphicTexture.h"

/// @brief ���_���C�A�E�g
enum InputLayoutKind
{
	VTX_COL,
	VTX_POLYGON,
	SKIN,
	EXPLOSION,
	MAX
};

//-----
// �e�풸�_���
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