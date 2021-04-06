/**
 * @file DX11Light.cpp
 * @brief ���C�g
 */
#include "DX11Light.h"
#include <Camera.h>

#define LIGHT_FOV ((60 * 3.14f) / 180)
#define LIGHT_ASPECT ( 1200 / 750 )
#define LIGHT_FAR (5000.0f)
        
 /**
  * @brief �R���X�g���N�^
  */
DX11Light::DX11Light()
	: m_type(Type::DIRECTIONAL)
	, m_pConstantBufferLight(NULL)
{
	m_lightBuf.eyePos.w = 1.f;
}
/**
 * @brief �f�X�g���N�^
 */
DX11Light::~DX11Light()
{
	Uninit();
}

 /**
  * @brief ������
  * @return ��������
  */
HRESULT DX11Light::Init()
{
	HRESULT hr = E_FAIL;
	if (m_pConstantBufferLight != NULL) {
		return hr;
	}

	// �o�b�t�@�쐬
	D3D11_BUFFER_DESC transformBufDesc;
	transformBufDesc.ByteWidth = sizeof(ConstantLightBuffer);
	transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
	transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformBufDesc.CPUAccessFlags = 0;
	transformBufDesc.MiscFlags = 0;
	transformBufDesc.StructureByteStride = 0;
	hr = GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &m_pConstantBufferLight);

	return hr;
}

/**
 * @brief �I������
 */
void DX11Light::Uninit()
{
	SAFE_RELEASE(m_pConstantBufferLight);
}

/**
 * @brief �X�V����
 */
#include"../DXg11/Input.h"
void DX11Light::Update()
{
#define _SPD (5.0f)
	/*
	if (Input::GetKey(DIK_V, Input::HOLD)) {
		m_lightBuf.eyePos.x += _SPD;
	}
	if (Input::GetKey(DIK_B, Input::HOLD)) {
		m_lightBuf.eyePos.x -= _SPD;
	}
	if (Input::GetKey(DIK_N, Input::HOLD)) {
		m_lightBuf.eyePos.z += _SPD;
	}
	if (Input::GetKey(DIK_M, Input::HOLD)) {
		m_lightBuf.eyePos.z -= _SPD;
	}
	if (Input::GetKey(DIK_K, Input::HOLD)) {
		m_lightBuf.eyePos.y += _SPD;
	}
	if (Input::GetKey(DIK_L, Input::HOLD)) {
		m_lightBuf.eyePos.y -= _SPD;
	}
	*/

	CreateMatrix();
	CreateProjectionMatrix();
	GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferLight, 0, NULL, &m_lightBuf, 0, 0);
}

/**
 * @brief �����ʒu�ݒ�
 * @param[in] pos �ʒu
 */
void DX11Light::SetPos(const DirectX::XMFLOAT3 &pos)
{
	m_lightBuf.lightVec.x = pos.x;
	m_lightBuf.lightVec.y = pos.y;
	m_lightBuf.lightVec.z = pos.z;
	m_lightBuf.lightVec.w = 1.f; // w=1�̎��͈ʒu
}
/**
 * @brief ���������ݒ�
 * @param[in] dir ����
 */
void DX11Light::SetDirection(const DirectX::XMFLOAT3 &dir)
{
	m_lightBuf.lightVec.x = dir.x;
	m_lightBuf.lightVec.y = dir.y;
	m_lightBuf.lightVec.z = dir.z;
	m_lightBuf.lightVec.w = 0.f; // w=0�̎��͕���
}
/**
 * @brief ���_�ʒu�ݒ�
 * @param[in] eyepos
 */
void DX11Light::SetEyePos(const DirectX::XMFLOAT3 &eyepos)
{
	m_lightBuf.eyePos.x = eyepos.x;
	m_lightBuf.eyePos.y = eyepos.y;
	m_lightBuf.eyePos.z = eyepos.z;
}
/**
* @brief �L�������ݒ�
* @param[in] bEnable
*/
void DX11Light::SetEnable(bool bEnable)
{
	m_lightBuf.eyePos.w = (bEnable) ? 1.0f : 0.0f;
}

void DX11Light::CreateMatrix() {
	DirectX::XMVECTOR Eye = 
		DirectX::XMVectorSet(GetEyePos().x + Camera::Instance()->paramPosition.x, 
			GetEyePos().y, GetEyePos().z + Camera::Instance()->paramPosition.z,
			0.0f);


	DirectX::XMVECTOR At = DirectX::XMVectorSet(
	//	GetEyePos().x + GetDirection().x*10000.0f,
	//	GetEyePos().y - GetDirection().y*10000.0f,
	//	GetEyePos().z + GetDirection().z*10000.0f, 0.0f);
		-1 + Camera::Instance()->paramPosition.x, -1, 0 + Camera::Instance()->paramPosition.z ,0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX lightMat;
	lightMat = DirectX::XMMatrixLookAtLH(Eye, At, Up);	

	DirectX::XMStoreFloat4x4(&world, lightMat);
}

void DX11Light::CreateProjectionMatrix() {

	DirectX::XMMATRIX setprojection;

	setprojection = DirectX::XMMatrixPerspectiveFovLH(LIGHT_FOV, 1.0f, 1300.0f, LIGHT_FAR);

	DirectX::XMStoreFloat4x4(&projection, setprojection);

}

// EOF