#pragma once

#include <DXg11/GameObjectList.h>

class Barrier :public GameObject {
public:
	Barrier();
	~Barrier() { pTexture = nullptr; };

	void Update();
	void LateDraw();
	bool isOn;
private:
	GraphicTexture graphicTexture;
	GraphicTexture randomGraphicTexture;

	ID3D11ShaderResourceView *pTexture;
	ID3D11ShaderResourceView *pRandomTexture;


};

