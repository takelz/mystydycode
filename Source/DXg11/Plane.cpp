#include "Plane.h"
#include "../Camera.h"
#include <list>

#define PLANE_NUM_BLOCK (10)


static const char *POLYGON_VERTEX_SHADER = "Assets/Shader/polygon_vs.cso";
static const char *POLYGON_PIXEL_SHADER = "Assets/Shader/polygon_ps.cso";

 ID3D11Buffer*			Plane::pBufferVertex;
 ID3D11Buffer*			Plane::pBufferMatrix;
 ID3D11VertexShader*	Plane::pShaderVertex;
 ID3D11PixelShader*		Plane::pShaderPixel;
 ID3D11InputLayout*		Plane::pFormat;
 ID3D11SamplerState*	Plane::pSamplerPoint;
 ID3D11SamplerState*	Plane::pSamplerLinear;

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMFLOAT4 color;
};

void Plane::InitializeVertex() {
	ID3D11Device *pDevice = GetDX11Device();

	// 頂点バッファ作成
	VertexPOLYGON *pVtx = CalcVertexPoint();


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

VertexPOLYGON* Plane::CalcVertexPoint() {

	VertexPOLYGON retVtx[(PLANE_NUM_BLOCK * 2) * (PLANE_NUM_BLOCK + 1)];

	int numBlockZ = PLANE_NUM_BLOCK;
	int numBlockX = PLANE_NUM_BLOCK;
	float sizeBlockX = 10;
	float sizeBlockZ = 10;
	const float sizeTexX = 1.0f * numBlockX;
	const float sizeTexZ = 1.0f * numBlockZ;

	int arrayCnt = 0;
	for (int nCntVtxZ = 0; nCntVtxZ < numBlockZ; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (numBlockX + 1); nCntVtxX++)
		{
			VertexPOLYGON vtx[2];
			// 頂点座標の設定
			vtx[0].pos.x = -(numBlockX / 2.0f) * sizeBlockX + nCntVtxX * sizeBlockX;
			vtx[0].pos.y = 0.0f;
			vtx[0].pos.z = (numBlockZ / 2.0f) * sizeBlockZ - (nCntVtxZ + 1) * sizeBlockZ;
			vtx[1].pos.x = -(numBlockX / 2.0f) * sizeBlockX + nCntVtxX * sizeBlockX;
			vtx[1].pos.y = 0.0f;
			vtx[1].pos.z = (numBlockZ / 2.0f) * sizeBlockZ - nCntVtxZ * sizeBlockZ;

			// 法線の設定
			//pVtx[0].nor = DirectX::XMFLOAT3(0.0f, 1.0, 0.0f);
			//pVtx[1].nor = DirectX::XMFLOAT3(0.0f, 1.0, 0.0f);

			// 反射光の設定
			vtx[0].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vtx[1].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			vtx[0].uv.x = sizeTexX * nCntVtxX;
			vtx[0].uv.y = sizeTexZ * (nCntVtxZ + 1);
			vtx[1].uv.x = sizeTexX * nCntVtxX;
			vtx[1].uv.y = sizeTexZ * nCntVtxZ;

			retVtx[arrayCnt]		= vtx[0];
			retVtx[arrayCnt + 1]	= vtx[1];
			arrayCnt += 2;


		}
	}




	return retVtx;
}

void Plane::SetTexture(const char* name) {
	SetTexture(TEXTURE->Load(name));
}

void Plane::SetTexture(Texture* pTexture)
{
	pTexture = pTexture;
}

using namespace DirectX;
void Plane::Draw(Transform trans) {
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