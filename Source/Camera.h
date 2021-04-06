/*
*--- FileName ---
* CCamera.h
*
*--- Auther ---
* ITO GO
*
*--- Overview ---
* メインカメラのクラス
*
*--- Update ---
* 2019/05/??    佐々木先生        ベース作成
* 2019/05/10    伊藤 豪            シングルトンの機構改変。関数内でstaticのCCameraを宣言するのではなく、メンバ変数として宣言した。
* 2019/05/20    伊藤 豪            移動用の関数作成。
* 2019/05/28	武田 正光          paramPositionとparamRotationで簡単にカメラ操作できるように。しかしMoveAxis系の関数が機能しなくなりました。1
* 2019/06/07	武田 正光          lookModeがonならparamLookに設定した地点を注視するように
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
    DirectX::XMFLOAT3        position;        // カメラ位置
    DirectX::XMFLOAT3        oldPosition;    // カメラの1フレーム前の座標
    DirectX::XMFLOAT3        lookAt;            // 注視点
    DirectX::XMFLOAT3        oldLookAt;        // 1フレーム前の注視点
    DirectX::XMFLOAT3        up;                // 上向きベクトル

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
    bool lookMode;				// rotationを無効にしlookAtに設定した位置を見続ける
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
    float paramRotLookLength;	// 注視点を設定していないとき。デフォルト1000
    void SetOnLookMode();		// Rotation関係なく注視点の位置で向きを決める
    void SetOffLookMode();		// Rotationでカメラの向きを決める
    bool GetLookMode() { return lookMode; }	// 現在注視モードオンならtrueを返す
    DirectX::Vector3 GetLookVector();	// 現在向いている方向の向きベクトルを取得(とりあえず動く用)

private:
    // 操作するためのパラメータを追加
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