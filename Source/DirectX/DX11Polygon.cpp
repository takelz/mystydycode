/**
 * @file DX11Polygon.cpp
 * @brief ポリゴン
 * @author ryota.ssk
 */
#include <DirectX/DX11Polygon.h>
#include <Debug/DebugUtility.h>
#include <Shader/ShaderSelector.h>

using namespace DirectX;

static const char *POLYGON_VERTEX_SHADER = "Assets/Shader/polygon_vs.cso";
static const char *POLYGON_PIXEL_SHADER = "Assets/Shader/polygon_ps.cso";



DX11PolygonInterface* DX11PolygonInterface::m_pInstance = NULL;

struct MatrixBuffer
{
	XMFLOAT4X4 matrix;
	XMFLOAT4 color;
};

/**
 * @brief コンストラクタ
 */
DX11Polygon::DX11Polygon(DimensionKind kind, float w, float h)
	: m_dimension(kind)
	, m_interpolation(POINT)
	, m_pTexture(NULL)
	, m_size(w, h)
	, m_pos(0, 0, 0)
	, m_scale(1, 1)
	, m_color(1,1,1,1)
{
}
DX11Polygon::DX11Polygon(DimensionKind kind, DirectX::XMFLOAT2 size)
	:DX11Polygon(kind, size.x, size.y)
{
}
/**
 * @brief デストラクタ
 */
DX11Polygon::~DX11Polygon()
{
	TEXTURE->Release(m_pTexture);
}

void DX11Polygon::Draw()
{
	POLYGON->Draw(*this);
}

void DX11Polygon::SetTexture(const char *file)
{
	SetTexture(TEXTURE->Load(file));
}
void DX11Polygon::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}
void DX11Polygon::SetInterpolation(InterpolationKind kind)
{
	m_interpolation = kind;
}
void DX11Polygon::SetAnchor(AnchorKind kind)
{
	switch (kind)
	{
	case LeftTop:
		m_anchor.x = 0.5f;
		m_anchor.y = 0.5f;
		break;
	default:
	case Center:
		m_anchor.x = 0.f;
		m_anchor.y = 0.f;
		break;
	}
}
void DX11Polygon::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}
void DX11Polygon::SetScale(float x, float y)
{
	m_scale.x = x;
	m_scale.y = y;
}
void DX11Polygon::SetColor(float r, float g, float b, float a)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_color.w = a;
}






DX11PolygonInterface* DX11PolygonInterface::Instance()
{
	return m_pInstance;
}
void DX11PolygonInterface::Create()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = NEW DX11PolygonInterface();
	}
}
void DX11PolygonInterface::Delete()
{
	SAFE_DELETE(m_pInstance);
}

DX11PolygonInterface::DX11PolygonInterface()
{
	ID3D11Device *pDevice = GetDX11Device();

	const float screenW = 1200;
	const float screenH = 750;
	m_MatrixOrthgonal = XMMatrixOrthographicOffCenterLH(
		0, screenW, screenH, 0, -1, 1);
	m_MatrixPerspective = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60), screenW / screenH, 1, 1000);


	VertexPOLYGON vtx[] = {
		{ { 0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 1,0 } },
		{ { 0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 1,1 } },
		{ { -0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 0,0 } },
		{ { -0.5f,-0.5f, 0 },{ 1,1,1,1 },{ 0,0 } },
		{ { 0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 1,1 } },
		{ { -0.5f, 0.5f, 0 },{ 1,1,1,1 },{ 0,1 } },
	};

	// 頂点バッファ作成
	D3D11_BUFFER_DESC vtxBufferDesc;
	vtxBufferDesc.ByteWidth = sizeof(vtx);
	vtxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxBufferDesc.MiscFlags = 0;
	vtxBufferDesc.StructureByteStride = 0;
	vtxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vtxBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vtxResourceData;
	vtxResourceData.pSysMem = vtx;
	vtxResourceData.SysMemPitch = 0;
	vtxResourceData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&vtxBufferDesc, &vtxResourceData, &m_pBufferVertex);

	// マトリックスバッファ
	D3D11_BUFFER_DESC matrixBufDesc;
	matrixBufDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufDesc.CPUAccessFlags = 0;
	matrixBufDesc.MiscFlags = 0;
	matrixBufDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&matrixBufDesc, NULL, &m_pBufferMatrix);

	// シェーダ作成
	CreateVertexShader(
		POLYGON_VERTEX_SHADER, InputLayoutKind::VTX_POLYGON,
		&m_pShaderVertex, &m_pFormat);
	CreatePixelShader(POLYGON_PIXEL_SHADER, &m_pShaderPixel);
	

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
	pDevice->CreateSamplerState(&pointDesc, &m_pSamplerPoint);
	pDevice->CreateSamplerState(&linearDesc, &m_pSamplerLinear);
}
DX11PolygonInterface::~DX11PolygonInterface()
{
	SAFE_RELEASE(m_pSamplerLinear);
	SAFE_RELEASE(m_pSamplerPoint);
	SAFE_RELEASE(m_pShaderPixel);
	SAFE_RELEASE(m_pShaderVertex);
	SAFE_RELEASE(m_pBufferMatrix);
	SAFE_RELEASE(m_pBufferVertex);
	SAFE_RELEASE(m_pFormat);
}
void DX11PolygonInterface::SetOrthgonal(const XMMATRIX& orth)
{
	m_MatrixOrthgonal = orth;
}
void DX11PolygonInterface::SetPerspective(const XMMATRIX& pers)
{
	m_MatrixPerspective = pers;
}
const XMMATRIX& DX11PolygonInterface::GetOrthgoanl()
{
	return m_MatrixOrthgonal;
}
const XMMATRIX& DX11PolygonInterface::GetPerspective()
{
	return m_MatrixPerspective;
}

void DX11PolygonInterface::Draw(const DX11Polygon& obj)
{
	ID3D11Device *pDevice = GetDX11Device();
	ID3D11DeviceContext *pDeviceContext = GetDX11DeviceContext();

	// 各シェーダセット
	TextureShaderSelector::Get()->SetFormat();
	TextureShaderSelector::Get()->SetShader();


	// 変換行列
	XMMATRIX anchor = XMMatrixTranslation(obj.m_anchor.x, obj.m_anchor.y, 0);
	XMMATRIX size = XMMatrixScaling(obj.m_size.x * obj.m_scale.x, obj.m_size.y * obj.m_scale.y, 1);
	XMMATRIX pos = XMMatrixTranslation(obj.m_pos.x, obj.m_pos.y, 0);
	XMMATRIX mat = anchor * size * pos;
	XMMATRIX view =
		obj.m_dimension == DX11Polygon::D2 ? 
			m_MatrixOrthgonal : m_MatrixPerspective;

	// バッファ
	MatrixBuffer data;
	XMStoreFloat4x4(&data.matrix, XMMatrixTranspose(
		XMMatrixMultiply(mat, view)));
	data.color = obj.m_color;
	pDeviceContext->UpdateSubresource(m_pBufferMatrix, 0, NULL, &data, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pBufferMatrix);

	// 頂点情報
	UINT strides = sizeof(VertexPOLYGON);
	UINT offsets = 0;
	pDeviceContext->IASetInputLayout(m_pFormat);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pBufferVertex, &strides, &offsets);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pDeviceContext->VSSetShader(m_pShaderVertex, NULL, 0);

	// ピクセルシェーダ
	//pDeviceContext->PSSetShader(m_pShaderPixel, NULL, 0);
	pDeviceContext->PSSetShaderResources(0, 1, &obj.m_pTexture);
	pDeviceContext->PSSetSamplers(0, 1,
		&(obj.m_interpolation == DX11Polygon::POINT ?
			m_pSamplerPoint : m_pSamplerLinear));

	// 描画
	pDeviceContext->Draw(6, 0);
}

// EOF