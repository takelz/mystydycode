#include"RenderModel.h"
#include"../Camera.h"
#include"RenderTargetManager.h"
#include<Shader/ShadowWrite.h>

using namespace DirectX;

#pragma region	// ModelObjEx



// コンストラクタ
ModelObjEx::ModelObjEx(){
	radius = 1.0f;

}

// デストラクタ
ModelObjEx::~ModelObjEx(){

}

// ファイルから生成
bool ModelObjEx::Create(const char* file, bool isDynamic)
{
	bool result = ModelObject::Create(file, isDynamic);

	// 境界ボックス サイズ取得
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
	// 境界ボックス/境界球中心座標取得
	vCenter.x = (vMax.x + vMin.x) / 2.0f;
	vCenter.y = (vMax.y + vMin.y) / 2.0f;
	vCenter.z = (vMax.z + vMin.z) / 2.0f;
	// 境界球半径取得
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


// レイとメッシュの当たり判定
bool ModelObjEx::Intersect(const DirectX::XMFLOAT3* pRayPos,const DirectX::XMFLOAT3* pRayDir,
							float* pfDist,DirectX::XMFLOAT3* pNormal){
	XMVECTOR vW = XMLoadFloat3(pRayDir);
	XMFLOAT3 P1, P2, P3;
	XMVECTOR vN, vV1, vV2, vV3, vV0, vN0;
	float dot, t, dividend, divisor;

	for (int mesh = 0; mesh < GetMeshCount(); ++mesh) {
		for (int polygon = 0; polygon < GetPolygonCount(mesh); ++polygon) {
			// 三角形の頂点座標
			P1 = GetVertexInfo(mesh, polygon, 0)->pos;
			P2 = GetVertexInfo(mesh, polygon, 1)->pos;
			P3 = GetVertexInfo(mesh, polygon, 2)->pos;
			// 三角形の辺のベクトル
			vV1 = XMVectorSet(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z, 0.0f);
			vV2 = XMVectorSet(P3.x - P2.x, P3.y - P2.y, P3.z - P2.z, 0.0f);
			vV3 = XMVectorSet(P1.x - P3.x, P1.y - P3.y, P1.z - P3.z, 0.0f);
			// 面の法線ベクトル
			vN = XMVector3Normalize(XMVector3Cross(vV1, vV2));

			// レイのベクトルと法線ベクトルの内積
			XMStoreFloat(&divisor, XMVector3Dot(vN, vW));
			if (divisor == 0.0f) {	// レイと面が平行→交点が存在しない
				continue;
			}
			if (divisor > 0.0f) {	// レイと法線が同じ方向→裏から表へ
				continue;
			}

			// レイのベクトルと三角形の辺1で構成する平面の法線ベクトル
			vN0 = XMVector3Cross(vV1, vW);
			// 三角形の頂点1からレイの始点へのベクトル
			vV0 = XMVectorSet(pRayPos->x - P1.x, pRayPos->y - P1.y, pRayPos->z - P1.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// 三角形の外側
				continue;
			}
			// レイのベクトルと三角形の辺2で構成する平面の法線ベクトル
			vN0 = XMVector3Cross(vV2, vW);
			// 三角形の頂点2からレイの始点へのベクトル
			vV0 = XMVectorSet(pRayPos->x - P2.x, pRayPos->y - P2.y, pRayPos->z - P2.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// 三角形の外側
				continue;
			}
			// レイのベクトルと三角形の辺3で構成する平面の法線ベクトル
			vN0 = XMVector3Cross(vV3, vW);
			// 三角形の頂点3からレイの始点へのベクトル
			vV0 = XMVectorSet(pRayPos->x - P3.x, pRayPos->y - P3.y, pRayPos->z - P3.z, 0.0f);
			XMStoreFloat(&dot, XMVector3Dot(vN0, vV0));
			if (dot < 0.0f) {		// 三角形の外側
				continue;
			}

			// レイの始点から平面上の任意の点へのベクトル
			vV0 = XMVectorSet(P1.x - pRayPos->x, P1.y - pRayPos->y, P1.z - pRayPos->z, 0.0f);
			XMStoreFloat(&dividend, XMVector3Dot(vN, vV0));
			t = dividend / divisor;
			if (t < 0.0f) {			// 交点がレイの後ろ
				continue;
			}

			// 交点あり
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

// 文字列で探してない場合は読み込み
ModelObjEx* ModelStorage::ReadModel(const char *name) {
	std::string str = name;
	auto count = storageList.count(str);

	if (!count) {	// 見つからなかったら
		ModelObjEx *obj = new ModelObjEx();
		if (obj->Create(name)) {
			storageList[str] = obj;
			return obj;
		}else {
			MessageBox(NULL, name,"モデルを読み込めませんでした" ,MB_OK);
			return nullptr;
		}
	}		
	else {	// 見つかったらマップの中の同じモデルを渡す
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
	// モデル読み込み
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
		modelTrans = {	// 影描画用
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
		// 貼り付ける影のテクスチャ
		GetDX11DeviceContext()->PSSetShaderResources(1, 1,&ShadowWrite::Get()->graphic.pTexture);
	}

	

	XMMATRIX mtxCalc, mtxScl, mtxPos,mtxRot;
	// ワールドマトリックスの初期化
	mtxCalc = XMMatrixIdentity();



#pragma region

	//// 回転を反映

	//// ヨーピッチロール
	//mtxCalc = mtxCalc,XMMatrixRotationRollPitchYaw(
	//	XMConvertToRadians(trans.rotation.x), XMConvertToRadians(trans.rotation.y), XMConvertToRadians(trans.rotation.z));

	////ワールド座標回転用
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

	//// スケールを反映
	//mtxScl = XMMatrixScaling(trans.scale.x, trans.scale.y, trans.scale.z);
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxScl);

	//// 移動を反映
	//mtxPos = XMMatrixTranslation(trans.position.x, trans.position.y, trans.position.z);
	//mtxCalc = XMMatrixMultiply(mtxCalc, mtxPos);

	//XMStoreFloat4x4(&mtxWorld, mtxCalc);

#pragma endregion

	// マトリックス更新
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
	// ワールドマトリックスの初期化
	mtxCalc = XMMatrixIdentity();
	// マトリックス更新
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