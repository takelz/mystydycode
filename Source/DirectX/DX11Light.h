/**
 * @file DX11Light.h
 * @brief ライト
 */
#ifndef __DX11_LIGHT_H__
#define __DX11_LIGHT_H__


#include "DX11Util.h"


class DX11Light
{
private:
	struct ConstantLightBuffer {
		DirectX::XMFLOAT4 lightVec;
		DirectX::XMFLOAT4 eyePos;
	};

public:
	enum class Type {
		DIRECTIONAL,
		POINT,
		SPOT
	};

public:
	DX11Light();
	~DX11Light();

	HRESULT Init();
	void Uninit();
	void Update();

	//! @name Set function
	/// @{
	void SetPos(const DirectX::XMFLOAT3 &pos);
	void SetDirection(const DirectX::XMFLOAT3 &dir);
	void SetEyePos(const DirectX::XMFLOAT3 &eyepos);
	void SetEnable(bool bEnable = true);

	const DirectX::XMFLOAT4X4& GetMatrix() {
		CreateMatrix();
		return world;
	}
	const DirectX::XMFLOAT4X4& GetProjectionMatrix() {
		return projection;
	}


	// 行列関係
	void CreateMatrix();
	void CreateProjectionMatrix();
	/// @}

	//! @name Get function
	/// @{
	ID3D11Buffer *const *GetBuffer() { return &m_pConstantBufferLight; }
	const DirectX::XMFLOAT4& GetDirection()
	{
		return m_lightBuf.lightVec;
	}
	const DirectX::XMFLOAT4& GetEyePos()
	{
		return m_lightBuf.eyePos;
	}
	/// @}

	DirectX::XMFLOAT3 offsetCameraPos;	// カメラに追従していく

private:
	Type				m_type;
	ConstantLightBuffer m_lightBuf;
	ID3D11Buffer*       m_pConstantBufferLight;
	DirectX::XMFLOAT4X4        world;
	DirectX::XMFLOAT4X4        projection;
};

#endif // __DX11_LIGHT_H__
