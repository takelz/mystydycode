#include"Transform.h"
#include"../Camera.h"

using namespace DirectX;


Transform::Transform() {
	instanceID = -1;
	scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f; scale = 1.0f;
	parentTransform = nullptr;	

}

float Transform::GetHighestScale() {
	if (scale.x > scale.y && scale.x > scale.z) return scale.x;
	else if (scale.y > scale.x && scale.y > scale.z) return scale.y;
	else return scale.z;

}

void Transform::UpdateMatrix() {
	XMMATRIX mtxParent,mtxCalc, mtxScl, mtxPos;
	// ワールドマトリックスの初期化
	mtxParent = XMMatrixIdentity();
	mtxCalc = XMMatrixIdentity();

	if (parentTransform) {
		XMMATRIX pMat = XMMatrixIdentity();
		// 
		mtxParent = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(parentTransform->rotation.x), 
			XMConvertToRadians(parentTransform->rotation.y), 
			XMConvertToRadians(parentTransform->rotation.z));

		// スケールは反映しない
		mtxScl = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		mtxParent = XMMatrixMultiply(mtxCalc, mtxScl);

		mtxPos = XMMatrixTranslation(parentTransform->position.x, parentTransform->position.y, parentTransform->position.z);
		mtxParent = XMMatrixMultiply(mtxCalc, mtxPos);

	}

	// 回転を反映
	mtxCalc = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));

	// スケールを反映
	mtxScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxCalc = XMMatrixMultiply(mtxCalc, mtxScl);

	// 移動を反映
	mtxPos = XMMatrixTranslation(position.x, position.y, position.z);
	mtxCalc = XMMatrixMultiply(mtxCalc, mtxPos);
	
	/*
	XMMATRIX lookMtx;
	XMFLOAT3 a;
	if (parentTransform) {	
		XMVECTOR Eye = DirectX::XMVectorSet(parentTransform->position.x, parentTransform->position.y, parentTransform->position.z, 0.0f);
		XMVECTOR At = DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f);
		XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		lookMtx = DirectX::XMMatrixLookAtLH(Eye, At, Up);
		

	}
	*/
	

	// もし親がいるなら
	if (parentTransform) mtxCalc = XMMatrixMultiply(mtxCalc,mtxParent);


	XMStoreFloat4x4(&matrix, mtxCalc);
	
	updatedMatrix = true;
}


void Transform::ParentTransform(Transform *transform) {
	parentTransform = transform;
}

void Transform::ParentDisconnect() {
	parentTransform = nullptr;
}

void Transform::CompareParameter() {
	if (position == prevPosition && rotation == prevRotation && scale == prevScale) {
		updatedMatrix = true;
	} 
	else { updatedMatrix = false; }

}

Vector3 Transform::Forward() {
	CompareParameter();
	if (!updatedMatrix) UpdateMatrix();
	return Vector3(matrix._31, matrix._32, matrix._33).Normalize();
}


Vector3 Transform::Back() {
	CompareParameter();
	if (!updatedMatrix) UpdateMatrix();
	return Vector3(-matrix._31, -matrix._32,-matrix._33).Normalize();
}


Vector3 Transform::Right() {
	CompareParameter();
	if (!updatedMatrix) UpdateMatrix();
	return Vector3(matrix._11, matrix._12, matrix._13).Normalize();
}


Vector3 Transform::Left() {
	CompareParameter();
	if (!updatedMatrix) UpdateMatrix();
	return Vector3(-matrix._11, -matrix._12, -matrix._13).Normalize();
}


