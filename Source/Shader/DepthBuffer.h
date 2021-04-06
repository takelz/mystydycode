#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class DepthBuffer {

public:
	DepthBuffer() { Create(); };

	// �T�C�Y�w��͖�����
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