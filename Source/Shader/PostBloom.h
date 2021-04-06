#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class PostBloom {
	struct Buffer {
		DirectX::XMFLOAT2 power;
		DirectX::XMFLOAT2 damy;
	};

public:
	PostBloom() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static PostBloom* Get() {
		static PostBloom instance;
		return &instance;
	}

	GraphicTexture lastResult;
	GraphicTexture brightTex;	// 輝度値が高いものだけを抽出


private:
	DX11Polygon *polygon;

	ID3D11Buffer* constBuffer;
	Buffer setBuffer;
private:

	PostBloom(const PostBloom&) = delete;
	PostBloom& operator=(const PostBloom&) = delete;
	PostBloom(PostBloom&&) = delete;
	PostBloom& operator=(PostBloom&&) = delete;


};