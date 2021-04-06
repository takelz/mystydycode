#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class NormalRender {

public:
	NormalRender() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static NormalRender* Get() {
		static NormalRender instance;
		return &instance;
	}

	GraphicTexture graphic;
private:
	DX11Polygon *polygon;


private:

	NormalRender(const NormalRender&) = delete;
	NormalRender& operator=(const NormalRender&) = delete;
	NormalRender(NormalRender&&) = delete;
	NormalRender& operator=(NormalRender&&) = delete;


};