#include"EnemyBullet.h"
#include"Src_Enemy/EnemyBase.h"
#include"../Src_Player/Player.h"

#define SPEED (50.0f)
#define END_TIME (240)

EnemyBullet::EnemyBullet() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/missile/sparrow.fbx");

	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());

}

void EnemyBullet::Update() {

	

	if (GetCollider()->Collide()) {
		if (GetCollider()->retColl->Tag() == TAG_PLAYER) {
			if (auto coll = D_CAST(Player, GetCollider()->retColl)) {
				coll->AddDamage();
				Destroy();
			}
		}
		if (GetCollider()->retColl->Tag() == TAG_PROTECED_OBJ) {
			if (auto coll = D_CAST(ProtectedObject, GetCollider()->retColl)) {
				coll->AddDamage();
				Destroy();
			}
		}
	}
	End();
	Move();



}
