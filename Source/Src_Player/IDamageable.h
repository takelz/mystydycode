#pragma once
#include"DXg11/GameObjectList.h"

// インターフェース
class IDamageable  {
public:

	virtual ~IDamageable() {};

	virtual void AddDamage() = 0;
private:

};





