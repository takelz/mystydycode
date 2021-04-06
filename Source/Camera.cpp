#include "Camera.h"
#include"DirectX/DX11Light.h"

#define PI ( 3.1415926f)  

using namespace DirectX;


void Camera::Update(void)
{
    this->UpPosition();

	//GetDX11Light()->SetEyePos(position);
}
void Camera::UpPosition(void)
{
	if (!notUpdate) {
		AddParamData();


		SetCamera(position, lookAt, up);
	}
	// �V�F�[�_�ɓn��Parameter�ݒ�
	cameraBuf.cameraPos.x = position.x;
	cameraBuf.cameraPos.y = position.y;
	cameraBuf.cameraPos.z = position.z;
	Vector3 direction = GetLookVector();
	cameraBuf.cameraVec.x = direction.x;
	cameraBuf.cameraVec.y = direction.y;
	cameraBuf.cameraVec.z = direction.z;
	GetDX11DeviceContext()->UpdateSubresource(pConstantBufferCamera, 0, NULL, &cameraBuf, 0, 0);
}


void Camera::AddParamData() {
	// �����_of��]
	if (!lookMode) {
		lookAt.x = position.x + (sinf(XMConvertToRadians(paramRotation.y)) * paramRotLookLength);
		lookAt.y = position.y + (sinf(XMConvertToRadians(paramRotation.x)) * paramRotLookLength);
		lookAt.z = position.z + (cosf(XMConvertToRadians(paramRotation.y)) + (cosf(XMConvertToRadians(paramRotation.y)) * paramRotLookLength));
		paramLookAt.x = lookAt.x;
		paramLookAt.y = lookAt.y;
		paramLookAt.z = lookAt.z;
	}
	else {
		lookAt.x=paramLookAt.x;
		lookAt.y=paramLookAt.y;
		lookAt.z=paramLookAt.z;
	}

	// �ʒu
	position.x = paramPosition.x;
	position.y = paramPosition.y;
	position.z = paramPosition.z;

	// Z��]�B0�Œn�ʂɐ����ȏ�Ԃɂ��邽�߂�+90�B
	up.x = cosf((paramRotation.z + 90 * PI) / 180);
	up.y = sinf((paramRotation.z + 90 * PI) / 180);



}

void Camera::SetOnLookMode() {
	lookMode = true;
}

void Camera::SetOffLookMode() {
	// �I�u�ɂ����Ƃ��Ɍ����Ă����������̂܂܂ɂȂ�悤�Ɋp�x��ݒ�(�͂܂�������)
	lookMode = false;

	//Vector3 dir = paramPosition - paramLookAt;
	//float leng = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	//dir.x /= leng; dir.y /= leng; dir.z /= leng;

}

Vector3 Camera::GetLookVector() {
	Vector3 dir =  paramLookAt - paramPosition;
	// ���K��
	float leng = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	dir.x /= leng;
	dir.y /= leng;
	dir.z /= leng;
	return dir;
}


//======================================================================
//
// End of File...
//
//======================================================================