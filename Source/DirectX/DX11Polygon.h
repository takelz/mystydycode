/**
 * @file DX11Polygon.h
 * @brief �|���S������
 * @author ryota.ssk
 */
#ifndef __DX11_POLYGON_H__
#define __DX11_POLYGON_H__

#include <DirectX/DX11Util.h>
#include <DirectX/DX11Texture.h>

#define POLYGON DX11PolygonInterface::Instance()

class DX11PolygonInterface;

 /**
 * @brief �|���S�����
 */
class DX11Polygon
{
	friend class DX11PolygonInterface;

	//----------
	// �\����
	//----------
public:
	/// @brief �`�掟��
	enum DimensionKind
	{
		D2,	// 2D
		D3	// 3D
	};
	/// @brief ��ԕ��@
	enum InterpolationKind
	{
		POINT,	// ��ԂȂ�
		LINEAR	// ���`���
	};
	/// @brief �|���S����_
	enum AnchorKind
	{
		LeftTop,	// ����
		Center,		// ����
	};

	//----------
	// �֐�
	//----------
public:
	DX11Polygon(DimensionKind kind, float w, float h);
	DX11Polygon(DimensionKind kind, DirectX::XMFLOAT2 size);
	~DX11Polygon();
	void Draw();

	void SetTexture(const char *file);
	void SetTexture(Texture* pTexture);
	void SetInterpolation(InterpolationKind kind);
	void SetAnchor(AnchorKind kind);
	void SetPos(float x, float y, float z);
	void SetScale(float x, float y);
	void SetColor(float r, float g, float b, float a);
	DimensionKind m_dimension;

private:

	InterpolationKind m_interpolation;
	Texture* m_pTexture;
	DirectX::XMFLOAT2 m_size;
	DirectX::XMFLOAT2 m_anchor;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT2 m_scale;
	DirectX::XMFLOAT4 m_color;
};

/**
* @brief �|���S���`��S��
*/
class DX11PolygonInterface
{
	//----------
	// �֐�
	//----------
public:
	static DX11PolygonInterface* Instance();
	static void Create();
	static void Delete();
public:
	DX11PolygonInterface();
	~DX11PolygonInterface();
	void SetOrthgonal(const DirectX::XMMATRIX& orth);
	void SetPerspective(const DirectX::XMMATRIX& pers);

	const DirectX::XMMATRIX& GetOrthgoanl();
	const DirectX::XMMATRIX& GetPerspective();

	void Draw(const DX11Polygon& obj);

	//----------
	// �ϐ�
	//----------
private:
	static DX11PolygonInterface* m_pInstance;
private:
	DirectX::XMMATRIX	m_MatrixOrthgonal;
	DirectX::XMMATRIX	m_MatrixPerspective;
	ID3D11InputLayout*	m_pFormat;
	ID3D11Buffer*		m_pBufferVertex;
	ID3D11Buffer*		m_pBufferMatrix;
	ID3D11VertexShader*	m_pShaderVertex;
	ID3D11PixelShader*	m_pShaderPixel;
	ID3D11SamplerState*	m_pSamplerPoint;
	ID3D11SamplerState*	m_pSamplerLinear;
};

#endif // __DX11_POLYGON_H__