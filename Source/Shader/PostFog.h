#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class PostFog {

public:
	PostFog() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static PostFog* Get() {
		static PostFog instance;
		return &instance;
	}

	GraphicTexture graphic;;


private:
	DX11Polygon *polygon;


private:

	PostFog(const PostFog&) = delete;
	PostFog& operator=(const PostFog&) = delete;
	PostFog(PostFog&&) = delete;
	PostFog& operator=(PostFog&&) = delete;


};