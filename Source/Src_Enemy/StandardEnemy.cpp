
#include"StandardEnemy.h"

#include"../Src_Player/Player.h"
#include<Src_Effect/Explosion.h>

#define POS_Y (50.0f)
#define SPEED (3.6f)

using namespace DirectX;

StandardEnemy::StandardEnemy() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/Enemy/air.fbx");

	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());

	transform.scale = 50;
	SetTag(TAG_ENEMY);

	

}

void StandardEnemy::Start() {
	auto itr = GameObjectList::Get()->GetTagList(TAG_PLAYER)->begin();
	AddProtectedObjData();
}

void StandardEnemy::Update() {
	FirstFall(POS_Y);

	
	Collide();
	LookTarget(*dest);
	MoveAttackPoint(*dest, SPEED);
}

void StandardEnemy::Draw() {
	GetModel()->Draw(transform);

	debug::DrawShapeArg arg;
	arg.shape = debug::DrawShapeArg::ShapeType::ST_BOX;
	arg.pose = debug::DrawShapeArg::PoseType::PT_ROTATION;
	arg.center = debug::DrawShapeArg::CenterType::CT_ORIGIN;

	XMFLOAT3 center(GetModel()->GetCenter().x, GetModel()->GetCenter().y, GetModel()->GetCenter().z);
	XMMATRIX move = XMMatrixTranslationFromVector(XMLoadFloat3(&center));
	arg.uPose.mat = XMMatrixMultiply(move, XMLoadFloat4x4(&transform.GetMatrix()));
	arg.uSize.vec = XMFLOAT3(GetModel()->GetBBox().x, GetModel()->GetBBox().y, GetModel()->GetBBox().z);
	arg.uSize.vec.x *= 2.0f;
	arg.uSize.vec.y *= 2.0f;
	arg.uSize.vec.z *= 2.0f;
	DrawShape(arg, XMFLOAT4(0, 1, 0, 1));
}

void StandardEnemy::AddProtectedObjData() {
	protectedObjList = GameObjectList::Get()->GetTagList(TAG_PROTECED_OBJ);
	dest = &protectedObjList->front()->transform.position;

}

void StandardEnemy::Collide() {
	if (GetCollider()->Collide()) {
		for (int i = 0; i < GetCollider()->retCollList.size(); i++) {
			//if ((GetCollider()->retCollList[i]->Tag() == TAG_PLAYER) || (GetCollider()->retCollList[i]->Tag() == TAG_UNIT)) {
				if (auto coll = D_CAST(IDamageable, GetCollider()->retCollList[i])) {
					coll->AddDamage();
					Destroy();
				}
			//}
			if (GetCollider()->retCollList[i]->Tag() == TAG_PROTECED_OBJ) {
				if (auto coll = D_CAST(ProtectedObject, GetCollider()->retCollList[i])) {
					AttackProtetedObj(coll);
					Destroy();


				}
			}
		}
		
	}
}

void StandardEnemy::AddDamage(int num) {
	Destroy();
	GameObject* obj = new Coin();
	GameObjectList::Get()->Generate(obj);
	obj->transform.position = transform.position;
	
	Explosion* efc = new Explosion();
	efc->Play(transform.position);
	GameObjectList::Get()->Generate(efc);

}