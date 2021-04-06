#pragma once
#include<DXg11/GameObjectList.h>
#include"../Src_Player/ProtectedObject.h"

#define DEFAULT_FALL_POS (2000)

class EnemyBase :public GameObject {
public:
	EnemyBase();
	virtual ~EnemyBase() { existNum--; };

	virtual void AddDamage(int num) = 0;
	static int GetNum() { return existNum; };
protected:
	// float height:ŒÅ’è‚·‚éˆÊ’u
	void FirstFall(float height);
	void MoveAttackPoint(DirectX::Vector3 destination,float speed = 1.0f);
	void AttackProtetedObj(ProtectedObject* obj);
	void LookTarget(DirectX::Vector3 obj);

	int life;

	static int existNum;		// ‘¶İ‚µ‚Ä‚¢‚é”

private:
	float fallAcc;				// Œø‰Ê—p‚Ì‰Á‘¬
};

