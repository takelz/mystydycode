#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"
// �[�x�o�b�t�@�̕`�����ݗp

class ShadowWrite {

public:
	ShadowWrite() {};

	// �T�C�Y�w��͖�����
	void Create(UINT width,UINT height);

	void Render();

	static ShadowWrite* Get() {
		static ShadowWrite instance;
		return &instance;
	}
	GraphicTexture graphic;

private:
	DX11Polygon *polygon;


private:

	ShadowWrite(const ShadowWrite&) = delete;
	ShadowWrite& operator=(const ShadowWrite&) = delete;
	ShadowWrite(ShadowWrite&&) = delete;
	ShadowWrite& operator=(ShadowWrite&&) = delete;


};