#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class PostBloom {
	struct Buffer {
		DirectX::XMFLOAT2 power;
		DirectX::XMFLOAT2 damy;
	};

public:
	PostBloom() { Create(); };

	// �T�C�Y�w��͖�����
	void Create();

	void Render();

	static PostBloom* Get() {
		static PostBloom instance;
		return &instance;
	}

	GraphicTexture lastResult;
	GraphicTexture brightTex;	// �P�x�l���������̂����𒊏o


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