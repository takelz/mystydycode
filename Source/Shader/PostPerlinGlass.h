#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class PostPerlinGlass {
	struct Buffer {
		DirectX::XMFLOAT2 time;
		DirectX::XMFLOAT2 damy;
	};
public:
	PostPerlinGlass() { Create(); };

	// �T�C�Y�w��͖�����
	void Create();

	void Render();

	static PostPerlinGlass* Get() {
		static PostPerlinGlass instance;
		return &instance;
	}

	GraphicTexture noise;

	GraphicTexture noiseR;
	GraphicTexture noiseG;
	GraphicTexture noiseB;

	bool renderGlassFilter;
private:
	DX11Polygon *polygon;
	

	ID3D11Buffer* constBuffer;
	Buffer setBuffer;
private:

	PostPerlinGlass(const PostPerlinGlass&) = delete;
	PostPerlinGlass& operator=(const PostPerlinGlass&) = delete;
	PostPerlinGlass(PostPerlinGlass&&) = delete;
	PostPerlinGlass& operator=(PostPerlinGlass&&) = delete;


};