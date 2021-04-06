#include "Shader.h"
#include <stdio.h>

HRESULT GetInputLayout(InputLayoutKind kind, const D3D11_INPUT_ELEMENT_DESC **ppLayout, UINT *pSize)
{
	static const D3D11_INPUT_ELEMENT_DESC skin_layout[] = {
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	static const D3D11_INPUT_ELEMENT_DESC vtxCol_layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const D3D11_INPUT_ELEMENT_DESC vtxColUV_layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const D3D11_INPUT_ELEMENT_DESC exp_layout[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	switch (kind) {
	default:
		return E_FAIL;
	case VTX_COL:
		*ppLayout = vtxCol_layout;
		*pSize = _countof(vtxCol_layout);
		break;
	case VTX_POLYGON:
		*ppLayout = vtxColUV_layout;
		*pSize = _countof(vtxColUV_layout);
		break;
	case SKIN:
		*ppLayout = skin_layout;
		*pSize = _countof(skin_layout);
		break;
	case EXPLOSION:
		*ppLayout = exp_layout;
		*pSize = _countof(exp_layout);
		break;
	}
	return S_OK;
}

HRESULT ReadFileData(LPCTSTR fileName, PBYTE *ppData, LPLONG pSize)
{
	FILE *fp = NULL;
	fopen_s(&fp, fileName, "rb");
	if (fp == NULL) {
		return E_FAIL;
	}
	
	fseek(fp, 0L, SEEK_END);
	*pSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	*ppData = new BYTE[*pSize];
	fread(*ppData, *pSize, 1, fp);
	fclose(fp);

	return S_OK;
}

HRESULT CreateVertexShader(LPCTSTR fileName, InputLayoutKind kind, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout)
{
	HRESULT hr = E_FAIL;

	BYTE *pData;
	long size;
	hr = ReadFileData(fileName, &pData, &size);
	if (FAILED(hr)) {
		SAFE_DELETE_ARRAY(pData);
		return hr;
	}

	ID3D11Device* pDevice = GetDX11Device();
	hr = pDevice->CreateVertexShader(pData, size, nullptr, ppVertexShader);
	if (FAILED(hr)) {
		SAFE_DELETE_ARRAY(pData);
		return hr;
	}

	const D3D11_INPUT_ELEMENT_DESC *layout = NULL;
	UINT num = 0;
	hr = GetInputLayout(kind, &layout, &num);
	if (FAILED(hr)) {
		SAFE_DELETE_ARRAY(pData);
		return hr;
	}

	hr = pDevice->CreateInputLayout(layout, num, pData, size, ppVertexLayout);

	SAFE_DELETE_ARRAY(pData);
	return hr;
}

HRESULT CreatePixelShader(LPCTSTR fileName, ID3D11PixelShader** ppPixelShader)
{
	HRESULT hr = E_FAIL;

	BYTE *pData;
	long size;
	hr = ReadFileData(fileName, &pData, &size);
	if (FAILED(hr)) {
		SAFE_DELETE_ARRAY(pData);
		return hr;
	}

	ID3D11Device* pDevice = GetDX11Device();
	hr = pDevice->CreatePixelShader(pData, size, nullptr, ppPixelShader);

	SAFE_DELETE_ARRAY(pData);
	return hr;
}