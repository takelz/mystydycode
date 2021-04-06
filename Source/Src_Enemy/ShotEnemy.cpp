
#include"ShotEnemy.h"
#include"../Src_Player/WalletMoney.h"
#include"../Src_Player/Player.h"
#include"EnemyBullet.h"

#define SHOT_DISTANCE (30.0f) // ‚Ç‚Ì‚®‚ç‚¢‹ß‚Ã‚¢‚½‚ç
#define SPEED (3.5f)
#define DEFAULT_LIFE (3)
#define CNT_NEXT_SHOT_FRAME (300)
#define OFFSET_GENERATE_BULLET (80)
#define POS_Y (60)
#define INTV_DAMAGE_TIME (120)


using namespace DirectX;

ShotEnemy::ShotEnemy() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/Enemy/shot/frigate01.fbx");
	//GetModel()->Load("Assets/ShotEnemy/Meshes/AlienFighter.fbx");

	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());


	transform.scale = 55.0f;
	SetTag(TAG_ENEMY);

	life = DEFAULT_LIFE;
	intvDamage = 0;

}

void ShotEnemy::Start() {
	auto itr = GameObjectList::Get()->GetTagList(TAG_PLAYER)->begin();
	AddProtectedObjData();
}

void ShotEnemy::Update() {
	FirstFall(POS_Y);

	ManageBullet();

	Collide();
	LookTarget(*dest);


}

void ShotEnemy::Draw() {

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

void ShotEnemy::ManageBullet() {

	if (CNT_NEXT_SHOT_FRAME < shotCnt) {
		if (Mathf::ComparePos(*dest, transform.position, SHOT_DISTANCE)) {
			shotCnt = 0;
			Shot();
		}
		else {
			MoveAttackPoint((*dest), SPEED);
		}
	}
	else { shotCnt++; }
}
void ShotEnemy::Shot() {
	// ”ÍˆÍ“à‚É“ü‚Á‚½‚çŽËŒ‚

		EnemyBullet *obj = new EnemyBullet();
		GameObjectList::Get()->Generate(obj);
		obj->transform.position = transform.position;
		obj->transform.rotation = transform.rotation;
		obj->direction = transform.Forward();

}

void ShotEnemy::AddProtectedObjData() {
	protectedObjList = GameObjectList::Get()->GetTagList(TAG_PROTECED_OBJ);
	dest = &protectedObjList->front()->transform.position;

}

void ShotEnemy::Collide() {
	intvDamage++;
	if (intvDamage < INTV_DAMAGE_TIME) return;
	intvDamage = 0;
	if (GetCollider()->Collide()) {
		for (int i = 0; i < GetCollider()->retCollList.size(); i++) {
			if (auto coll = D_CAST(IDamageable, GetCollider()->retCollList[i])) {
				coll->AddDamage();
			}

		}

	}
}

void ShotEnemy::AddDamage(int num) {
	life--;
	if (life <= 0) {
		Destroy();
		GameObject* obj = new Coin();
		GameObjectList::Get()->Generate(obj);
		obj->transform.position = transform.position;
	}
}