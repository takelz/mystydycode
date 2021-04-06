#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class PostFog {

public:
	PostFog() { Create(); };

	// �T�C�Y�w��͖�����
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