#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class PostNoiseWave {
	struct Buffer{
		DirectX::XMFLOAT2 time;
		DirectX::XMFLOAT2 damy;
	};
public:
	PostNoiseWave() { Create(); };

	// �T�C�Y�w��͖�����
	void Create();

	void Render();

	static PostNoiseWave* Get() {
		static PostNoiseWave instance;
		return &instance;
	}

	GraphicTexture graphic;;
	// �m�C�Y�G�t�F�N�g���N��
	void TriggerOn();
private:
	DX11Polygon *polygon;

	ID3D11Buffer* constBuffer;
	Buffer		setBuffer;
private:

	PostNoiseWave(const PostNoiseWave&) = delete;
	PostNoiseWave& operator=(const PostNoiseWave&) = delete;
	PostNoiseWave(PostNoiseWave&&) = delete;
	PostNoiseWave& operator=(PostNoiseWave&&) = delete;


};