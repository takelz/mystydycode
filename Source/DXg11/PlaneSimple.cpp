#include "PlaneSimple.h"
#include "../Camera.h"
#include <list>

#define PLANE_NUM_BLOCK (10)


static const char *POLYGON_VERTEX_SHADER = "Assets/Shader/polygon_vs.cso";
static const char *POLYGON_PIXEL_SHADER = "Assets/Shader/polygon_ps.cso";

 ID3D11Buffer*			PlaneSimple::pBufferVertex;
 ID3D11Buffer*			PlaneSimple::pBufferMatrix;
 ID3D11VertexShader*	PlaneSimple::pShaderVertex;
 ID3D11PixelShader*		PlaneSimple::pShaderPixel;
 ID3D11InputLayout*		PlaneSimple::pFormat;
 ID3D11SamplerState*	PlaneSimple::pSamplerPoint;
 ID3D11SamplerState*	PlaneSimple::pSamplerLinear;

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMFLOAT4 color;
};

void PlaneSimple::InitializeVertex() {
	ID3D11Device *pDevice = GetDX11Device();

	// 頂点バッファ作成
	VertexPOLYGON pVtx[] = {
		{ { 0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 1,0 } },
		{ { 0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 1,1 } },
		{ { -0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 0,0 } },
		{ { -0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 0,0 } },
		{ { 0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 1,1 } },
		{ { -0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 0,1 } },
	};



	D3D11_BUFFER_DESC vtxBufferDesc;
	vtxBufferDesc.ByteWidth = sizeof(pVtx);
	vtxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxBufferDesc.MiscFlags = 0;
	vtxBufferDesc.StructureByteStride = 0;
	vtxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vtxBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vtxResourceData;
	vtxResourceData.pSysMem = pVtx;
	vtxResourceData.SysMemPitch = 0;
	vtxResourceData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&vtxBufferDesc, &vtxResourceData, &pBufferVertex);

	// マトリックスバッファ
	D3D11_BUFFER_DESC matrixBufDesc;
	matrixBufDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufDesc.CPUAccessFlags = 0;
	matrixBufDesc.MiscFlags = 0;
	matrixBufDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&matrixBufDesc, NULL, &pBufferMatrix);

	CreateVertexShader(POLYGON_VERTEX_SHADER, InputLayoutKind::VTX_POLYGON,&pShaderVertex, &pFormat);
	CreatePixelShader(POLYGON_PIXEL_SHADER, &pShaderPixel);

	// サンプラ―
	D3D11_SAMPLER_DESC pointDesc;
	D3D11_SAMPLER_DESC linearDesc;
	ZeroMemory(&pointDesc, sizeof(D3D11_SAMPLER_DESC));
	ZeroMemory(&linearDesc, sizeof(D3D11_SAMPLER_DESC));
	pointDesc.AddressU = linearDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.AddressV = linearDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.AddressW = linearDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	linearDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	pDevice->CreateSamplerState(&pointDesc, &pSamplerPoint);
	pDevice->CreateSamplerState(&linearDesc, &pSamplerLinear);
}

void PlaneSimple::SetTexture(const char* name) {
	SetTexture(TEXTURE->Load(name));
}

void PlaneSimple::SetTexture(Texture* pTexture)
{
	pTexture = pTexture;
}

using namespace DirectX;
void PlaneSimple::Draw(Transform trans) {
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	// 変換行列
	XMMATRIX size = XMMatrixScaling(trans.scale.x,1.0f,trans.scale.z);
	XMMATRIX pos = XMMatrixTranslation(trans.position.x, trans.position.y, trans.position.z);
	XMMATRIX mat = size * pos;
	XMMATRIX view = XMLoadFloat4x4(&Camera::Instance()->GetCameraMatrix());

	// バッファ
	MatrixBuffer data;
	XMStoreFloat4x4(&data.matrix, XMMatrixTranspose(
		XMMatrixMultiply(mat, view)));
	data.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 色変更はなし
	pDeviceContext->UpdateSubresource(pBufferMatrix, 0, NULL, &data, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pBufferMatrix);

	// 頂点情報
	UINT strides = sizeof(VertexPOLYGON);
	UINT offsets = 0;
	pDeviceContext->IASetInputLayout(pFormat);
	pDeviceContext->IASetVertexBuffers(0, 1, &pBufferVertex, &strides, &offsets);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->VSSetShader(pShaderVertex, NULL, 0);
	
	// ピクセルシェーダ
	pDeviceContext->PSSetShader(pShaderPixel, NULL, 0);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	pDeviceContext->PSSetSamplers(0, 1,&pSamplerPoint);

	// 描画
	pDeviceContext->Draw(6, 0);
}