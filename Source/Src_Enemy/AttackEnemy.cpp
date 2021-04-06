
#include"AttackEnemy.h"

#include"../Src_Player/Player.h"
#include<Src_Effect/Explosion.h>

#define POS_Y (50.0f)
#define SPEED (5.0f)

using namespace DirectX;

AttackEnemy::AttackEnemy() {
	StandardEnemy::StandardEnemy();


}

void AttackEnemy::Start() {
	// ƒvƒŒƒCƒ„[‚Íˆê‚Â‚µ‚©‘¶Ý‚µ‚È‚¢
	auto itr = GameObjectList::Get()->GetTagList(TAG_PLAYER)->front();
	dest = &itr->transform.position;	

}

void AttackEnemy::Update() {
	FirstFall(POS_Y);
	
	
	Collide();
	LookTarget(*dest);
	MoveAttackPoint(*dest, SPEED);
}

void AttackEnemy::Draw() {
	// Ô‚­
	GetModel()->SetColor(1.0f, 0.2f, 0.1f);
	GetModel()->Draw(transform);

}


