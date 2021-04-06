#include"Collider.h"
#include"GameObjectList.h"

using namespace DirectX;

bool CircleCircle(Vector3 basePos, float baseRad, Vector3 targetPos, float targetRad);
bool AABBAABB(Vector3 basePos, Vector3 baseBox, Vector3 targetPos, Vector3 targetBox);
bool OBBOBB(XMFLOAT4X4 baseObj, Vector3 bsCenter, Vector3 bsBBox, XMFLOAT4X4 targetObj, Vector3 tgCenter, Vector3 tgBBox);

ColliderCircle::ColliderCircle():ColliderBase(){
	type = COLLIDER_CIRCLE;
}


ColliderCircle::~ColliderCircle() {

}

bool ColliderCircle::Collide() {
	auto itr = GameObjectList::Get()->objectList.begin();
	auto endPoint = GameObjectList::Get()->objectList.end();
	while (itr != endPoint) {
		if ((*itr)->transform.instanceID == transform->instanceID) {	// 自身と同じobjなら飛ばす
			++itr; continue;
		}
			if (auto target = (*itr)->pCollider) {		// この種類のコライダが存在しているか確認
				if (target->GetType() != COLLIDER_CIRCLE) continue;
				if (CircleCircle(transform->position, radius * transform->GetHighestScale(),						// 自身
						target->transform->position, target->radius * target->transform->GetHighestScale() )) {		// 相手
					retColl = (*itr);																	// 当たった相手のobjを返す
					return true;
				}

			}
			// 最後にイテレータを進める
			++itr;
		
	
	}
	return false;
}

ColliderAABB::ColliderAABB() :ColliderBase() {
	type = COLLIDER_AABB;
}


ColliderAABB::~ColliderAABB() {

}

bool ColliderAABB::Collide() {
	auto itr = GameObjectList::Get()->objectList.begin();
	auto endPoint = GameObjectList::Get()->objectList.end();
	while (itr != endPoint) {
		if ((*itr)->transform.instanceID == transform->instanceID) {	// 自身と同じobjなら飛ばす
			++itr; continue;
		}

			if (auto target = (*itr)->pCollider) {
				if (target->GetType() != COLLIDER_AABB) continue;
				if (AABBAABB(center, collBox * transform->GetHighestScale(),							// 自身
						target->center, target->collBox * target->transform->GetHighestScale() )) {	// 相手
					retColl = (*itr);
					return true;
				}

			}
		// 最後にイテレータを進める
		++itr;
	}


	return false;
}

ColliderOBB::ColliderOBB() :ColliderBase() {
	type = COLLIDER_OBB;
}


ColliderOBB::~ColliderOBB() {

}

bool ColliderOBB::Collide() {
	auto itr = GameObjectList::Get()->objectList.begin();
	auto endPoint = GameObjectList::Get()->objectList.end();
	int currentCollide = 0;	// この一回の当たり判定でどれだけ当たり判定するか
	bool retBool = false;

	retCollList.clear();
	// マトリックスを更新
	mtx = &transform->GetMatrix();

	while (itr != endPoint) {
		if ((*itr)->transform.instanceID == transform->instanceID) {	// 自身と同じobjなら飛ばす
			++itr; continue;
		}

		if (auto target = (*itr)->pCollider) {
			if (target->GetType() != COLLIDER_OBB) continue;
			target->mtx = &target->transform->GetMatrix();
			if (OBBOBB(*mtx,center, collBox ,										// 自身
						*target->mtx, target->center,target->collBox )) {	// 相手

				retColl = (*itr);
			
				retCollList.emplace_back((*itr));
				// 設定した値よりも大きくなったらループを抜ける
				if (maxCollide <= currentCollide) return true;
				else {
					currentCollide++;
					retBool = true;
				}
			}

		}
		// 最後にイテレータを進める
		++itr;

	}


	return retBool;
}



bool CircleCircle(Vector3 basePos, float baseRad, Vector3 targetPos, float targetRad) {
	float dx = basePos.x - targetPos.x;
	float dy = basePos.y - targetPos.y;
	float dz = basePos.z - targetPos.z;
	float dr = baseRad + targetRad;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

bool AABBAABB(Vector3 basePos, Vector3 baseBox, Vector3 targetPos, Vector3 targetBox) {
	// 境界ボックス サイズ取得
	Vector3 A = basePos;
	Vector3 B = targetPos;
	Vector3 Ar = baseBox;
	Vector3 Br = targetBox;
	// AABB同士の当たり判定
	return A.x - Ar.x <= B.x + Br.x && B.x - Br.x <= A.x + Ar.x &&
		A.y - Ar.y <= B.y + Br.y && B.y - Br.y <= A.y + Ar.y &&
		A.z - Ar.z <= B.z + Br.z && B.z - Br.z <= A.z + Ar.z;
}


// OBB同士の当たり判定
bool OBBOBB(XMFLOAT4X4 baseObj,Vector3 bsCenter,Vector3 bsBBox, XMFLOAT4X4 targetObj,Vector3 tgCenter,Vector3 tgBBox){

	// ワールド マトリックス取得
	XMFLOAT4X4 mWA = baseObj;
	XMFLOAT4X4 mWB = targetObj;
	// 境界ボックス中心座標をワールド座標に変換
	XMFLOAT3 A, B;
	XMVECTOR vCenter = XMLoadFloat3(&XMFLOAT3(bsCenter.x,bsCenter.y,bsCenter.z));
	XMMATRIX mWorld = XMLoadFloat4x4(&mWA);
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	XMStoreFloat3(&A, vCenter);
	vCenter = XMLoadFloat3(&XMFLOAT3(tgCenter.x, tgCenter.y, tgCenter.z));
	mWorld = XMLoadFloat4x4(&mWB);
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	XMStoreFloat3(&B, vCenter);
	// 中心座標間のベクトルを求める
	XMVECTOR vD = XMVectorSet(B.x - A.x, B.y - A.y, B.z - A.z, 0.0f);
	// モデル座標軸を求める
	XMVECTOR vN[6];
	vN[0] = XMVectorSet(mWA._11, mWA._12, mWA._13, 0.0f);
	vN[1] = XMVectorSet(mWA._21, mWA._22, mWA._23, 0.0f);
	vN[2] = XMVectorSet(mWA._31, mWA._32, mWA._33, 0.0f);
	vN[3] = XMVectorSet(mWB._11, mWB._12, mWB._13, 0.0f);
	vN[4] = XMVectorSet(mWB._21, mWB._22, mWB._23, 0.0f);
	vN[5] = XMVectorSet(mWB._31, mWB._32, mWB._33, 0.0f);
	// 境界ボックス サイズ取得
	const XMFLOAT3& Ar = XMFLOAT3(bsBBox.x,bsBBox.y,bsBBox.z);
	const XMFLOAT3& Br = XMFLOAT3(tgBBox.x, tgBBox.y, tgBBox.z);
	// OBBのサイズを表すベクトルを求める
	XMVECTOR vL[6];
	vL[0] = XMVectorSet(mWA._11 * Ar.x, mWA._12 * Ar.x, mWA._13 * Ar.x, 0.0f);
	vL[1] = XMVectorSet(mWA._21 * Ar.y, mWA._22 * Ar.y, mWA._23 * Ar.y, 0.0f);
	vL[2] = XMVectorSet(mWA._31 * Ar.z, mWA._32 * Ar.z, mWA._33 * Ar.z, 0.0f);
	vL[3] = XMVectorSet(mWB._11 * Br.x, mWB._12 * Br.x, mWB._13 * Br.x, 0.0f);
	vL[4] = XMVectorSet(mWB._21 * Br.y, mWB._22 * Br.y, mWB._23 * Br.y, 0.0f);
	vL[5] = XMVectorSet(mWB._31 * Br.z, mWB._32 * Br.z, mWB._33 * Br.z, 0.0f);
	// モデル座標軸が分離軸候補
	float fL, fD;
	for (int i = 0; i < 6; ++i) {
		XMVECTOR& vS = vN[i];	// 分離軸候補
		// OBBの影(の半分)の合計
		fL = 0.0f;
		for (int j = 0; j < 6; ++j) {
			XMStoreFloat(&fD, XMVector3Dot(vS, vL[i]));
			fL += fabsf(fD);
		}
		// 影と中心間の距離の比較
		XMStoreFloat(&fD, XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD) {
			return false;	// 当たっていない
		}
	}
	// 分離軸候補を2辺から生成
	XMVECTOR vS;	// 分離軸候補
	for (int i = 0; i < 3; ++i) {
		for (int j = 3; j < 6; ++j) {
			// 分離軸候補を求める
			vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));
			// OBBの影(の半分)の合計
			fL = 0.0f;
			for (int k = 0; k < 6; ++k) {
				XMStoreFloat(&fD, XMVector3Dot(vS, vL[k]));
				fL += fabsf(fD);
			}
			// 影と中心間の距離の比較
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) {
				return false;	// 当たっていない
			}
		}
	}
	return true;			// 当たっている
}
