#include"StandardBullet.h"
#include"Src_Enemy/EnemyBase.h"
#include<Src_Effect/Explosion.h>

#define SPEED (60.0f)
#define END_TIME (25)

StandardBullet::StandardBullet() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/missile/sparrow.fbx");

	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());
	transform.scale = 1.2f;
}

void StandardBullet::Update() {

	
	if (GetCollider()->Collide()) {
		if (GetCollider()->retColl->Tag() == TAG_ENEMY) {
			if (auto coll = D_CAST(EnemyBase, GetCollider()->retColl)) {
				coll->AddDamage(1);
				Destroy();
				Explosion* efc = new Explosion();
				efc->Play(transform.position);
				GameObjectList::Get()->Generate(efc);
			}
		}
	}

	End();
	Move();



}

void StandardBullet::Draw() {
	GetModel()->Draw(transform);

}


void StandardBullet::Move() {
	transform.position.x += direction.x * SPEED;
	transform.position.y += direction.y * SPEED;
	transform.position.z += direction.z * SPEED;
}

void StandardBullet::CollideObject() {

}

void StandardBullet::End() {
	if (endCnt > END_TIME) {
		Destroy();
	}
	else { endCnt++; }

	CollideObject();
}


//
//debug::DrawShapeArg arg;
//arg.shape = debug::DrawShapeArg::ShapeType::ST_BOX;
//arg.pose = debug::DrawShapeArg::PoseType::PT_ROTATION;
//arg.center = debug::DrawShapeArg::CenterType::CT_ORIGIN;
//
//XMFLOAT3 center(GetModel()->GetCenter().x, GetModel()->GetCenter().y, GetModel()->GetCenter().z);
//XMMATRIX move = XMMatrixTranslationFromVector(XMLoadFloat3(&center));
//arg.uPose.mat = XMMatrixMultiply(move, XMLoadFloat4x4(&transform.GetMatrix()));
//arg.uSize.vec = XMFLOAT3(GetModel()->GetBBox().x, GetModel()->GetBBox().y, GetModel()->GetBBox().z);
//arg.uSize.vec.x *= 2.0f;
//arg.uSize.vec.y *= 2.0f;
//arg.uSize.vec.z *= 2.0f;
//DrawShape(arg, XMFLOAT4(0, 1, 0, 1));