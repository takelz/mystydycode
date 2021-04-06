#pragma once

#include <DirectX/DX11Util.h>
#include <DirectX/DX11Texture.h>
#include "../Shader/Shader.h"
#include "Transform.h"

class Plane {
public:
	// í∏ì_ÇÃê›íËìô
	static void InitializeVertex();

	void Draw(DirectX::Transform trans);
	void SetTexture(const char *file);
	void SetTexture(Texture* pTexture);
private:
	static VertexPOLYGON* CalcVertexPoint();

	Texture*			pTexture;

	static ID3D11Buffer*		pBufferVertex;
	static ID3D11Buffer*		pBufferMatrix;
	static ID3D11VertexShader*	pShaderVertex;
	static ID3D11PixelShader*	pShaderPixel;
	static ID3D11InputLayout*	pFormat;
	static ID3D11SamplerState*	pSamplerPoint;
	static ID3D11SamplerState*	pSamplerLinear;

};