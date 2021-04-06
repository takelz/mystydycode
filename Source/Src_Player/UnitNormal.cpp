

#include"UnitNormal.h"
#include"../StandardBullet.h"

#define DEFAULT_UNIT_MAX_BULLET (5)
#define DEFAULT_UNIT_LIFE (3)
#define CNT_NEXT_FRAME (120)
#define OFFSET_GENERATE_BULLET (80)

int UnitNormal::bulletAddPower;
int UnitNormal::lifeAddPower;


UnitNormal::UnitNormal() {
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load("Assets/UNIT2/baseturret.fbx");
	transform.scale *= 1.0f;

	//base.Load("Assets/UNIT2/base.fbx");

	AddComponent(COMPONENT_COLLIDER_OBB);
	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());


	bulletNum = DEFAULT_UNIT_MAX_BULLET + bulletAddPower;
	life = DEFAULT_UNIT_LIFE + lifeAddPower;
	
	SetTag(TAG_UNIT);
}

void UnitNormal::Start() {
	baseTrans.scale = transform.scale = 30;
	transform.position.y = 0.0f ;
	baseTrans.position = transform.position;
}

void UnitNormal::Update() {
	ManageBullet();
	if (bulletNum <= 0 || life <= 0) {
		End();
	}
}

void UnitNormal::Draw() {

	GetModel()->Draw(transform);
}

void UnitNormal::Shot(){
	StandardBullet *obj = new StandardBullet();
	obj->transform.position = transform.position;
	obj->transform.position.y = OFFSET_GENERATE_BULLET;
	obj->transform.rotation = transform.rotation;
	obj->direction = transform.Forward().Normalize();
	GameObjectList::Get()->Generate(obj);

	bulletNum--;
}

void UnitNormal::ManageBullet(){
	if (CNT_NEXT_FRAME < shotCnt) {
		shotCnt = 0;
		Shot();
	}
	else { shotCnt++; }

}


void UnitNormal::End(){
	Destroy();
}


void UnitNormal::AddDamage() {
	life--;
}