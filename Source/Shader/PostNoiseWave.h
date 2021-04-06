#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class PostNoiseWave {
	struct Buffer{
		DirectX::XMFLOAT2 time;
		DirectX::XMFLOAT2 damy;
	};
public:
	PostNoiseWave() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static PostNoiseWave* Get() {
		static PostNoiseWave instance;
		return &instance;
	}

	GraphicTexture graphic;;
	// ノイズエフェクトを起動
	void TriggerOn();
private:
	DX11Polygon *polygon;

	ID3D11Buffer* constBuffer;
	Buffer		setBuffer;
private:

	PostNoiseWave(const PostNoiseWave&) = delete;
	PostNoiseWave& operator=(const PostNoiseWave&) = delete;
	PostNoiseWave(PostNoiseWave&&) = delete;
	PostNoiseWave& operator=(PostNoiseWave&&) = delete;


};