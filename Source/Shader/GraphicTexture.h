#pragma once
// モデルのシェーダー切替
// ＝＝＝＝＝＝＝＝＝＝
// ～概要～
// 新しいピクセルシェーダーを作る際
// Texture2D tex : register(t0);
// SamplerState samp : register(s0);
// この二つは定義しておく
// ConstantBufferMaterialと入力もベースのモデルシェーダーを同じ
// ConstantBufferMaterialは番号１からつかうこと。

#include <DirectX/DX11Light.h>
#include <unordered_map>
#include <DirectX/DirectXTex/WICTextureLoader.h>
#include <DirectX/DirectXTex/DirectXTex.h>




class GraphicTexture {
public:
	GraphicTexture() { pTexture = nullptr; };
	virtual ~GraphicTexture() {};

	bool Load(const char* file);

	ID3D11ShaderResourceView	*pTexture;

	void MakeTexDesc(D3D11_TEXTURE2D_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pData, UINT width, UINT height);
	HRESULT CreateResource(ID3D11Texture2D *pTex, const D3D11_TEXTURE2D_DESC &desc);

	bool CreateRenderTarget(UINT width, UINT height, const float inClearColor[4]);
	bool CreateDepthStencil(UINT width, UINT height, bool enableStencil);


	ID3D11RenderTargetView *pRenderTarget;
	ID3D11DepthStencilView *pDepthStencil;
	D3D11_VIEWPORT viewPort;
	float clearColor[4];

	D3D11_TEXTURE2D_DESC pDesc;
private:

};

