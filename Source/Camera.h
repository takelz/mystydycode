/*
*--- FileName ---
* CCamera.h
*
*--- Auther ---
* ITO GO
*
*--- Overview ---
* ���C���J�����̃N���X
*
*--- Update ---
* 2019/05/??    ���X�ؐ搶        �x�[�X�쐬
* 2019/05/10    �ɓ� ��            �V���O���g���̋@�\���ρB�֐�����static��CCamera��錾����̂ł͂Ȃ��A�����o�ϐ��Ƃ��Đ錾�����B
* 2019/05/20    �ɓ� ��            �ړ��p�̊֐��쐬�B
* 2019/05/28	���c ����          paramPosition��paramRotation�ŊȒP�ɃJ��������ł���悤�ɁB������MoveAxis�n�̊֐����@�\���Ȃ��Ȃ�܂����B1
* 2019/06/07	���c ����          lookMode��on�Ȃ�paramLook�ɐݒ肵���n�_�𒍎�����悤��
*/
#ifndef ___CAMERA_H___
#define ___CAMERA_H___
#include <directxmath.h>
#include "DXg11/Vector.h"
#include "Memory.h"
#include "DirectX/DX11Util.h"

#define DEFAULT_ROTLOOK_LENGHT (10000)

class Camera{
private:
	struct ConstantCameraBuffer {
		DirectX::XMFLOAT4 cameraVec;
		DirectX::XMFLOAT4 cameraPos;
	};


    DirectX::XMFLOAT4X4        projection;
    DirectX::XMFLOAT4X4        world;
    DirectX::XMFLOAT3        position;        // �J�����ʒu
    DirectX::XMFLOAT3        oldPosition;    // �J������1�t���[���O�̍��W
    DirectX::XMFLOAT3        lookAt;            // �����_
    DirectX::XMFLOAT3        oldLookAt;        // 1�t���[���O�̒����_
    DirectX::XMFLOAT3        up;                // ������x�N�g��

    DirectX::Vector3         translationEye;
    DirectX::Vector3         AccelerationEye;
    DirectX::Vector3         translationLookAt;
    DirectX::Vector3         AccelerationLookAt;

    float                    m_near;
    float                    m_Aspect;
    float                    m_Fov;
    float                    m_far;
    bool                    enableMovePosition;
    bool                    enableMoveLookAt;

public:
    DirectX::Vector3 paramPosition;
    DirectX::Vector3 paramRotation;
    DirectX::Vector3 paramLookAt;
	bool notUpdate;
private:
    bool lookMode;				// rotation�𖳌��ɂ�lookAt�ɐݒ肵���ʒu����������
	ConstantCameraBuffer cameraBuf;
	ID3D11Buffer*       pConstantBufferCamera;

public:

    Camera(){
		paramRotLookLength = DEFAULT_ROTLOOK_LENGHT;
		lookMode = false;
		lookAt = DirectX::XMFLOAT3(1.0f,1.0f,1.0f);
		paramLookAt = 1.0f;


    }
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;


	static Camera* Instance() {
		static Camera instance;
		return &instance;
	}



public:
    void Init(float nearclip, float farclip, float fov,
        float width, float height,
        DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookat, DirectX::XMFLOAT3 up){

        SetProjection(nearclip,farclip,fov,width,height);
        SetCamera(eye,lookat,up);

		D3D11_BUFFER_DESC transformBufDesc;
		transformBufDesc.ByteWidth = sizeof(ConstantCameraBuffer);
		transformBufDesc.Usage = D3D11_USAGE_DEFAULT;
		transformBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		transformBufDesc.CPUAccessFlags = 0;
		transformBufDesc.MiscFlags = 0;
		transformBufDesc.StructureByteStride = 0;
		GetDX11Device()->CreateBuffer(&transformBufDesc, NULL, &pConstantBufferCamera);
    }
    void CreateCameraMatrix(){
        DirectX::XMVECTOR Eye = DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f);
        DirectX::XMVECTOR At = DirectX::XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0.0f);
        DirectX::XMVECTOR Up = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);

        DirectX::XMMATRIX camera;
        camera = DirectX::XMMatrixLookAtLH(Eye, At, Up);

        DirectX::XMStoreFloat4x4(&world, camera);
    }
    void CreateProjectionMatrix(){

        DirectX::XMMATRIX setprojection;

        setprojection = DirectX::XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

        DirectX::XMStoreFloat4x4(&projection, setprojection);

    }
    void Update(void);


public:
    float paramRotLookLength;	// �����_��ݒ肵�Ă��Ȃ��Ƃ��B�f�t�H���g1000
    void SetOnLookMode();		// Rotation�֌W�Ȃ������_�̈ʒu�Ō��������߂�
    void SetOffLookMode();		// Rotation�ŃJ�����̌��������߂�
    bool GetLookMode() { return lookMode; }	// ���ݒ������[�h�I���Ȃ�true��Ԃ�
    DirectX::Vector3 GetLookVector();	// ���݌����Ă�������̌����x�N�g�����擾(�Ƃ肠���������p)

private:
    // ���삷�邽�߂̃p�����[�^��ǉ�
    void AddParamData();



private:

    void UpPosition(void);


public:
	ID3D11Buffer *const *GetBuffer() { 	
		return &pConstantBufferCamera; 
	}

    const DirectX::XMFLOAT4X4& GetCameraMatrix(){
        return world;
    }
    const DirectX::XMFLOAT4X4& GetProjectionMatrix(){
        return projection;
    }
    float GetFov() const {
        return m_Fov;
    }
    const DirectX::XMFLOAT3& GetEye() const {
        return position;
    }
    const DirectX::XMFLOAT3& GetLookat() const {
        return lookAt;
    }
    const DirectX::XMFLOAT3& GetUp() const {
        return up;
    }

public:
    void SetNear(float nearclip){
        m_near = nearclip;
    }
    void SetFar(float farclip){
        m_far = farclip;
    }
    void SetFov(float fov){
        m_Fov = fov;
    }
    void SetAspect(float width,float height){
        m_Aspect = width/height;
    }
    void SetProjection(float nearclip,float farclip,float fov,float width,float height){
        SetNear(nearclip);
        SetFar(farclip);
        SetFov(fov);
        SetAspect(width,height);
        CreateProjectionMatrix();
    }
    void SetCamera(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& lookat, const DirectX::XMFLOAT3& up){

        SetEye(eye);
        SetLookat(lookat);
        SetUp(up);
        CreateCameraMatrix();
    }
    void SetEye(DirectX::XMFLOAT3 eye){
        position = { eye.x,eye.y,eye.z };
    }
    void SetLookat(const DirectX::XMFLOAT3& setlookat){
        lookAt = setlookat;
    }
    void SetUp(const DirectX::XMFLOAT3& setup){
        up = setup;
    }


};

#endif//!___CAMERA_H___