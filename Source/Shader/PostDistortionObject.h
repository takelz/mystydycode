#pragma once
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Polygon.h>
#include "GraphicTexture.h"

// ���ʂ̃����_�[���ʂ�\��

class PostDistortionObject {
	struct Buffer {
		DirectX::XMFLOAT4 cameraDir;
	};

public:
	PostDistortionObject() { Create(); };

	// �T�C�Y�w��͖�����
	void Create();

	void Render();

	static PostDistortionObject* Get() {
		static PostDistortionObject instance;
		return &instance;
	}

	GraphicTexture distNormal;
	GraphicTexture distTex;		// �`����䂪�܂�
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