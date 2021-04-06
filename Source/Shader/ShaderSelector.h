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

#include "Shader.h"
#include <DirectX/DX11Light.h>
#include <unordered_map>


enum SHADER {
	SHADER_MODEL,
	SHADER_BLUE,
	SHADER_CRYSTAL,
	SHADER_BARRIER,
	SHADER_SHADOW,
	SHADER_DEPTH_WRITE,
	SHADER_DEPTH_BUFFER,
	SHADER_NORMAL,
	SHADER_DAMAGE_EMISSION,
	SHADER_COIN,

	SHADER_TEX_DEFAULT,
	SHADER_TEX_BLUR,
	SHADER_TEX_GETBRIGHT,
	SHADER_TEX_BLOOM,
	SHADER_TEX_FOG,
	SHADER_TEX_BLOCKNOISE,
	SHADER_TEX_DISTORTION,
	SHADER_TEX_PERLIN_NOISE,
	SHADER_TEX_GLASS_FILTER,
	SHADER_MAX,
};

class ShaderData {
public:
	ShaderData() {
		format = nullptr;
		pVertexShader = nullptr;
		pPixelShader = nullptr;
	}
	~ShaderData() {
		if (format) delete format;
		if (pVertexShader) delete pVertexShader;
		if (pPixelShader) delete pPixelShader;
	}

	ID3D11InputLayout	*format;
	ID3D11VertexShader	*pVertexShader;
	ID3D11PixelShader	*pPixelShader;
private:
};

class ShaderSelector {
	
public:
	ShaderSelector(const ShaderSelector&) = delete;
	ShaderSelector& operator=(const ShaderSelector&) = delete;
	ShaderSelector(ShaderSelector&&) = delete;
	ShaderSelector& operator=(ShaderSelector&&) = delete;

	static ShaderSelector* Get() {
		static ShaderSelector instance;
		return &instance;
	}

	void Setup();
		
	// 各オブヘクトのシェーダ切り替え、モデルシェーダでないならスルー
	void SelectModelShader(SHADER shader);	
	// 全体へ付与させるのシェーダ
	void SelectModelShaderRender(SHADER shader);


	// GetDX11DeviceContext()と一緒、PSとVSをセット。SHADER_MODELならなにもいらない
	ID3D11DeviceContext* SetConstantBuffers() { return GetDX11DeviceContext(); };	
	void SetFormat();
	void SetShader();

	

private:
	ShaderSelector();
	ShaderData *currentData;
	SHADER currentShaderName;

	std::unordered_map<SHADER,ShaderData*> dataList;

};

//////////////////////////////////////////
//////////////////////////////////////////


class TextureShaderSelector {

public:
	TextureShaderSelector(const TextureShaderSelector&) = delete;
	TextureShaderSelector& operator=(const TextureShaderSelector&) = delete;
	TextureShaderSelector(TextureShaderSelector&&) = delete;
	TextureShaderSelector& operator=(TextureShaderSelector&&) = delete;

	static TextureShaderSelector* Get() {
		static TextureShaderSelector instance;
		return &instance;
	}

	void Setup();

	// 各オブヘクトのシェーダ切り替え、
	void SelectShader(SHADER shader);
	

	// GetDX11DeviceContext()と一緒、PSとVSをセット。SHADER_MODELならなにもいらない
	ID3D11DeviceContext* SetConstantBuffers() { return GetDX11DeviceContext(); };
	void SetFormat();
	void SetShader();



private:
	TextureShaderSelector();
	ShaderData *currentData;
	SHADER currentShaderName;

	std::unordered_map<SHADER, ShaderData*> dataList;

};

