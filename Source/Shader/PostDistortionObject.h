#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// 普通のレンダー結果を表示

class PostDistortionObject {
	struct Buffer {
		DirectX::XMFLOAT4 cameraDir;
	};

public:
	PostDistortionObject() { Create(); };

	// サイズ指定は無効化
	void Create();

	void Render();

	static PostDistortionObject* Get() {
		static PostDistortionObject instance;
		return &instance;
	}

	GraphicTexture distNormal;
	GraphicTexture distTex;		// 描画をゆがます
private:
	DX11Polygon *polygon;


	ID3D11Buffer* constBuffer;
	Buffer setBuffer;
private:

	PostDistortionObject(const PostDistortionObject&) = delete;
	PostDistortionObject& operator=(const PostDistortionObject&) = delete;
	PostDistortionObject(PostDistortionObject&&) = delete;
	PostDistortionObject& operator=(PostDistortionObject&&) = delete;


};