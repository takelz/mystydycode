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
	// 構造体
	//----------
public:
	/// @brief 描画次元
	enum DimensionKind
	{
		D2,	// 2D
		D3	// 3D
	};
	/// @brief 補間方法
	enum InterpolationKind
	{
		POINT,	// 補間なし
		LINEAR	// 線形補間
	};
	/// @brief ポリゴン基準点
	enum AnchorKind
	{
		LeftTop,	// 左上
		Center,		// 中央
	};

	//----------
	// 関数
	//----------
public:

	void SetDimension(DimensionKind d);
	void SetInterpolation(InterpolationKind i);
	void SetAnchor(AnchorKind a);
	void SetColor(float r, float g, float b, float a);


	DirectX::XMFLOAT2 size;				// sizeは今のところ？？
	DirectX::Vector3 pos;
	DirectX::Vector3 scale;
	DirectX::XMFLOAT4 color;

private:
	DimensionKind dimension;			// 2dか3dか
	InterpolationKind interpolation;	// 補間方法

	DirectX::XMFLOAT2 anchor;
	DX11Polygon polygon;
	
};
