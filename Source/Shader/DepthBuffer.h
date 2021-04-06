#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class DepthBuffer {

public:
	DepthBuffer() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static DepthBuffer* Get() {
		static DepthBuffer instance;
		return &instance;
	}

	GraphicTexture graphic;;


private:
	DX11Polygon *polygon;


private:

	DepthBuffer(const DepthBuffer&) = delete;
	DepthBuffer& operator=(const DepthBuffer&) = delete;
	DepthBuffer(DepthBuffer&&) = delete;
	DepthBuffer& operator=(DepthBuffer&&) = delete;


};