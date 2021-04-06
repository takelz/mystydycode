
#include"EnemyBase.h"

#define DEC_SPD (0.93f)
#define ACC (30.0f)
#define DEC_SPD_POS (500)

using namespace DirectX;

int EnemyBase::existNum;

EnemyBase::EnemyBase() {
	existNum++;
	fallAcc = ACC;
}



void EnemyBase::FirstFall(float height) {
	if (transform.position.y > height) {
		transform.position.y -= fallAcc;
		if(transform.position.y < DEC_SPD_POS)
			fallAcc *= DEC_SPD;
	}
	else {
		transform.position.y = height;
	}
}


void EnemyBase::MoveAttackPoint(Vector3 destination,float speed) {
	Vector3 target = Mathf::DirectionVector2pt(destination, transform.position);

	transform.position.x += target.x * speed;
	transform.position.z += target.z * speed;

}

void EnemyBase::AttackProtetedObj(ProtectedObject* obj) {
	obj->AddDamage();
}

void EnemyBase::LookTarget(Vector3 obj){
	Vector3 dir;
	dir = obj - transform.position;
	transform.rotation.y = Mathf::VectorToAngleY(dir.Normalize());


	
}
