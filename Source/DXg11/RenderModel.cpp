#include"RenderModel.h"
#include"../Camera.h"
#include"RenderTargetManager.h"
#include<Shader/ShadowWrite.h>

using namespace DirectX;

#pragma region	// ModelObjEx



// �R���X�g���N�^
ModelObjEx::ModelObjEx(){
	radius = 1.0f;

}

// �f�X�g���N�^
ModelObjEx::~ModelObjEx(){

}

// �t�@�C�����琶��
bool ModelObjEx::Create(const char* file, bool isDynamic)
{
	bool result = ModelObject::Create(file, isDynamic);

	// ���E�{�b�N�X �T�C�Y�擾
	XMFLOAT3 vPos, vMin, vMax;
	vMin = vMax = GetVertexInfo(0, 0, 0)->pos;
	for (int mesh = 0; mesh < GetMeshCount(); ++mesh) {
		for (int polygon = 0; polygon < GetPolygonCount(mesh); ++polygon) {
			for (int index = 0; index < 3; ++index) {
				vPos = GetVertexInfo(mesh, polygon, index)->pos;
				if (vMin.x > vPos.x) vMin.x = vPos.x;
				if (vMin.y > vPos.y) vMin.y = vPos.y;
				if (vMin.z > vPos.z) vMin.z = vPos.z;
				if (vMax.x < vPos.x) vMax.x = vPos.x;
				if (vMax.y < vPos.y) vMax.y = vPos.y;
				if (vMax.z < vPos.z) vMax.z = vPos.z;
			}
		}
	}
	vBBox.x = (vMax.x - vMin.x) / 2.0f;
	vBBox.y = (vMax.y - vMin.y) / 2.0f;
	vBBox.z = (vMax.z - vMin.z) / 2.0f;
	// ���E�{�b�N�X/���E�����S���W�擾
	vCenter.x = (vMax.x + vMin.x) / 2.0f;
	vCenter.y = (vMax.y + vMin.y) / 2.0f;
	vCenter.z = (vMax.z + vMin.z) / 2.0f;
	// ���E�����a�擾
	radius = 0.0f;
	float dx, dy, dz, f;
	for (int mesh = 0; mesh < GetMeshCount(); ++mesh) {
		for (int polygon = 0; polygon < GetPolygonCount(mesh); ++polygon) {
			for (int index = 0; index < 3; ++index) {
				vPos = GetVertexInfo(mesh, polygon, index)->pos;
				dx = vPos.x - vCenter.x;
				dy = vPos.y - vCenter.y;
				dz = vPos.z - vCenter.z;
				f = sqrtf(dx * dx + dy * dy + dz * dz);
				if (radius < f) radius = f;
			}
		}
	}

	return result;
}


// ���C�ƃ��b�V���̓����蔻��
bool ModelObjEx::Intersect(const DirectX::XMFLOAT3* pRayPos,const DirectX::XMFLOAT3* pRayDir,
							float* pfDist,DirectX::XMFLOAT3* pNormal){
	XMVECTOR vW = XMLoadFloat3(pRayDir);
	XMFLOAT3 P1, P2, P3;
	XMVECTOR vN, vV1, vV2, vV3, vV0, vN0;
	float dot, t, dividend, divisor;

	for (int mesh = 0; mesh < GetMeshCount(); ++mesh) {
		for (int polygon = 0; polygon < GetPolygonCount(mesh); ++polygon) {
			// �O�p�`�̒��_���W
			P1 = GetVertexInfo(mesh, polygon, 0)->pos;
			P2 = GetVertexInfo(mesh, polygon, 1)->pos;
			P3 = GetVertexInfo(mesh, polygon, 2)->pos;
			// �O�p�`�̕ӂ̃x�N�g��
			vV1 = XMVectorSet(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z, 0.0f);
			vV2 = XMVectorSet(P3.x - P2.x, P3.y - P2.y, P3.z - P2.z, 0.0f);
			vV3 = XMVectorSet(P1.x - P3.x, P1.y - P3.y, P1.z - P3.z, 0.0f);
			// �ʂ̖@���x�N�g��
			vN = XMVector3Normalize(XMVector3Cross(vV1, vV2));

			// ���C�̃x�N�g���Ɩ@���x�N�g���̓���
			XMStoreFloat(&divisor, XMVector3Dot(vN, vW));
			if (divisor == 0.0f) {	// ���C�Ɩʂ����s����_�����݂��Ȃ�
				continue;
			}
			if (divisor > 0.0f) {	// ���C�Ɩ@��������������������\��
				continue;
			}

			// ���C�̃x�N�g���ƎO�p�`�̕�1�ō\�����镽�ʂ̖@���x�N�g��
			vN0 = XMVector3Cross(vV1, vW);
			// �O�p�`�̒��_1���烌�C�̎n�_�ւ̃x�N�g��
			vV0 = XMVectorSet(pRayPos->x - P1.x, pRayPos->y - P1.y, pRayPos->z - P1.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// �O�p�`�̊O��
				continue;
			}
			// ���C�̃x�N�g���ƎO�p�`�̕�2�ō\�����镽�ʂ̖@���x�N�g��
			vN0 = XMVector3Cross(vV2, vW);
			// �O�p�`�̒��_2���烌�C�̎n�_�ւ̃x�N�g��
			vV0 = XMVectorSet(pRayPos->x - P2.x, pRayPos->y - P2.y, pRayPos->z - P2.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// �O�p�`�̊O��
				continue;
			}
			// ���C�̃x�N�g���ƎO�p�`�̕�3�ō\�����镽�ʂ̖@���x�N�g��
			vN0 = XMVector3Cross(vV3, vW);
			// �O�p�`�̒��_3���烌�C�̎n�_�ւ̃x�N�g��
			vV0 = XMVectorSet(pRayPos->x - P3.x, pRayPos->y - P3.y, pRayPos->z - P3.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// �O�p�`�̊O��
				continue;
			}

			// ���C�̎n�_���畽�ʏ�̔C�ӂ̓_�ւ̃x�N�g��
			vV0 = XMVectorSet(P1.x - pRayPos->x, P1.y - pRayPos->y, P1.z - pRayPos->z, 0.0f);
			XMStoreFloat(&dividend, XMVector3Dot(vN, vV0));
			t = dividend / divisor;
			if (t < 0.0f) {			// ��_�����C�̌��
				continue;
			}

			// ��_����
			if (pfDist) {
				*pfDist = t;
			}
			if (pNormal) {
				XMStoreFloat3(pNormal, vN);
			}
			return true;
		}
	}
	return false;
}


#pragma endregion


#pragma region	// ModelStorage

std::unordered_map<std::string, ModelObjEx*> ModelStorage::storageList;

ModelStorage::ModelStorage() {

}

ModelStorage::~ModelStorage() {

}

// ������ŒT���ĂȂ��ꍇ�͓ǂݍ���
ModelObjEx* ModelStorage::ReadModel(const char *name) {
	std::string str = name;
	auto count = storageList.count(str);

	if (!count) {	// ������Ȃ�������
		ModelObjEx *obj = new ModelObjEx();
		if (obj->Create(name)) {
			storageList[str] = obj;
			return obj;
		}else {
			MessageBox(NULL, name,"���f����ǂݍ��߂܂���ł���" ,MB_OK);
			return nullptr;
		}
	}		
	else {	// ����������}�b�v�̒��̓������f����n��
		return storageList[name];
	}
}


#pragma endregion


RenderModel::RenderModel() {
	XMStoreFloat4x4(&mtxWorld, XMMatrixIdentity());
	model = nullptr;
}

void RenderModel::Load(const char *name) {
	if (model) return;
	// ���f���ǂݍ���
	model = ModelStorage::ReadModel(name);
	//if (!model->Create(name, true)) {
	//	MessageBox(NULL, "", "", MB_OK);
	//}
}

void RenderModel::Draw(Transform& trans) {
	DX11Graphics *pGraphics = GetDX11Graphics();
		
	ShaderSelector::Get()->SetFormat();
	ShaderSelector::Get()->SetShader();
	
	ModelObject::Transform modelTrans;

	if (RenderTargetManager::Get()->GetCurrentPath() == PATH_SHADOW_NUM) {
		modelTrans = {	// �e�`��p
			XMMatrixIdentity(),
			XMLoadFloat4x4(&GetDX11Light()->GetMatrix()),
			XMLoadFloat4x4(&GetDX11Light()->GetProjectionMatrix()),
		};
	}else {
		modelTrans = {
			XMMatrixIdentity(),
			XMLoadFloat4x4(&Camera::Instance()->GetCameraMatrix()),
			XMLoadFloat4x4(&Camera::Instance()->GetProjectionMatrix()),
		};
		// �\��t����e�̃e�N�X�`��
		GetDX11DeviceContext()->PSSetShaderResources(1, 1,&ShadowWrite::Get()->graphic.pTexture);
	}

	

	XMMATRIX mtxCalc, mtxScl, mtxPos,mtxRot;
	// ���[���h�}�g���b�N�X�̏�����
	mtxCalc = XMMatrixIdentity();



#pragma region

	//// ��]�𔽉f

	//// ���[�s�b�`���[��
	//mtxCalc = mtxCalc,XMMatrixRotationRollPitchYaw(
	//	XMConvertToRadians(trans.rotation.x), XMConvertToRadians(trans.rotation.y), XMConvertToRadians(trans.rotation.z));

	////���[���h���W��]�p
	///*
	//mtxRot = XMMatrixRotationY(XMConvertToRadians(trans.rotation.y));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);
	//mtxRot = XMMatrixRotationX(XMConvertToRadians(trans.rotation.x));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);
	//mtxRot = XMMatrixRotationZ(XMConvertToRadians(trans.rotation.z));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);
	//*/

	///*
	//XMFLOAT3 axis = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
	//XMVECTOR vec = XMLoadFloat3(&axis);
	//mtxRot = XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(trans.rotation.y));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);

	//axis = XMFLOAT3(mtxWorld._11, mtxWorld._12, mtxWorld._13);
	//vec = XMLoadFloat3(&axis);
	//mtxRot = XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(trans.rotation.x));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);

	//axis = XMFLOAT3(mtxWorld._31, mtxWorld._32, mtxWorld._33);
	//vec = XMLoadFloat3(&axis);
	//mtxRot = XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(trans.rotation.z));
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxRot);
	//*/
	//

	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(trans.scale.x, trans.scale.y, trans.scale.z);
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxScl);

	//// �ړ��𔽉f
	//mtxPos = XMMatrixTranslation(trans.position.x, trans.position.y, trans.position.z);
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxPos);

	//XMStoreFloat4x4(&mtxWorld, mtxCalc);

#pragma endregion

	// �}�g���b�N�X�X�V
	trans.UpdateMatrix();


	modelTrans.world = XMLoadFloat4x4(&trans.GetMatrix());
	
	model->Draw(modelTrans);
}


void RenderModel::Draw(Transform& trans, ModelAnimator *animator) {
	DX11Graphics *pGraphics = GetDX11Graphics();


	ModelObject::Transform modelTrans = {
		XMMatrixIdentity(),
		XMLoadFloat4x4(&Camera::Instance()->GetCameraMatrix()),
		XMLoadFloat4x4(&Camera::Instance()->GetProjectionMatrix()),
	};

	XMMATRIX mtxCalc, mtxScl, mtxPos, mtxRot;
	// ���[���h�}�g���b�N�X�̏�����
	mtxCalc = XMMatrixIdentity();
	// �}�g���b�N�X�X�V
	trans.UpdateMatrix();


	modelTrans.world = XMLoadFloat4x4(&trans.GetMatrix());
	//modelTrans.world = XMMatrixIdentity();
	if (animator) {
		model->Draw(modelTrans, animator);
	}else	model->Draw(modelTrans);
	


}

Vector3 RenderModel::GetCenter() {
	return Vector3(model->GetCenter().x, model->GetCenter().y, model->GetCenter().z);
}

Vector3 RenderModel::GetBBox() {
	return Vector3(model->GetBBox().x, model->GetBBox().y, model->GetBBox().z);
}


float RenderModel::GetRadius() {
 	return model->GetRadius();
}