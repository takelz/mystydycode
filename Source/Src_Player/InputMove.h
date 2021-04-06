#pragma once

#include "../DXg11/GameObjectList.h"

class InputMove {
public:
	InputMove();
	~InputMove() { trans = nullptr; };

	void Update();

	Transform *trans;
private:
	Vector3 acc;	// ‰Á‘¬“x

	void MoveRotate(float *rot,bool pls);
};