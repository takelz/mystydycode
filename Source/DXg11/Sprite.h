#pragma once


#include <DirectX/DX11Util.h>
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include <DirectX/DX11Polygon.h>
#include"Vector.h"
#include"Texture.h"



class Sprite {
	friend class SpriteManager;
public:
	Sprite();
	~Sprite();
	void Load(const char *name);
	void Render();



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

	void SetDimension(DimensionKind d);
	void SetInterpolation(InterpolationKind i);
	void SetAnchor(AnchorKind a);
	void SetColor(float r, float g, float b, float a);


	DirectX::XMFLOAT2 size;				// size�͍��̂Ƃ���H�H
	DirectX::Vector3 pos;
	DirectX::Vector3 scale;
	DirectX::XMFLOAT4 color;

private:
	DimensionKind dimension;			// 2d��3d��
	InterpolationKind interpolation;	// ��ԕ��@

	DirectX::XMFLOAT2 anchor;
	DX11Polygon polygon;
	
};
