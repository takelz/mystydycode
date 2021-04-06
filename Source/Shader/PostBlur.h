#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

#define BLUR_SAMPLE_NUM (10)

class Blur {
	struct Buffer {
		DirectX::XMFLOAT2 power;
		DirectX::XMFLOAT2 damy;
	};

public:
	Blur() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();
	void BlurTexture(GraphicTexture *pTex);

	static Blur* Get() {
		static Blur instance;
		return &instance;
	}

	GraphicTexture graphic;
	GraphicTexture down;
	GraphicTexture up;


private:
	struct SAMPLE {
		DX11Polygon *polygon;
		GraphicTexture graphic;
	};

	SAMPLE downSamp[BLUR_SAMPLE_NUM];
	SAMPLE upSamp[BLUR_SAMPLE_NUM];


	DX11Polygon *polygon;
	DX11Polygon *downPolygon;
	DX11Polygon *upPolygon;

	ID3D11Buffer* constBuffer;
	Buffer setBuffer;
private:

	Blur(const Blur&) = delete;
	Blur& operator=(const Blur&) = delete;
	Blur(Blur&&) = delete;
	Blur& operator=(Blur&&) = delete;


};