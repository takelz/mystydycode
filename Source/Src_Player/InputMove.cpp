#include "InputMove.h"

#define MOVE_SPEED (10.0f)
#define MAX_ACC
#define MAX_ROTATE (10.0f)
#define ACC_ROTATE (0.2f)
#define RIMIT_DIST (3300)

InputMove::InputMove() {

}

void InputMove::Update() {
	Vector3 dir;
	bool pushed = false;;

	// 前後
	if (Input::GetKey(DIK_W, Input::HOLD) ||(DIGetPadStick(PAD_ST_L_Y) < -0.2f)) {

		dir += trans->Forward();
		pushed = true;
		

	}

	if (Input::GetKey(DIK_S, Input::HOLD) || (DIGetPadStick(PAD_ST_L_Y) > 0.2f)) {
		dir += trans->Back();
		pushed = true;

	} 


	// 左へ。動けばいいコード
	if (Input::GetKey(DIK_A, Input::HOLD) ||(DIGetPadStick(PAD_ST_L_X) < -0.2f)) {
		dir += trans->Left();
		pushed = true;

		if (trans->rotation.z < MAX_ROTATE) {
			trans->rotation.z += ACC_ROTATE;
		}
	} else {
		if (trans->rotation.z > 0) {
			trans->rotation.z -= ACC_ROTATE * 2;
		}
	}
	// 右へ
	if (Input::GetKey(DIK_D, Input::HOLD) ||  (DIGetPadStick(PAD_ST_L_X) > 0.2f)){
		dir += trans->Right();
		pushed = true;
		if (trans->rotation.z > -MAX_ROTATE) {
			trans->rotation.z -= ACC_ROTATE;
		}
	} else {
		if (trans->rotation.z < 0) {
			trans->rotation.z += ACC_ROTATE * 2;
		}
	}


	if (!pushed){
		if (acc.Length() > 1.0f) acc *= 0.9f;
		else acc = 0.0f;
	}
	else {
		acc += dir.Normalize();
	}

	if ((acc.Length()) > MOVE_SPEED) acc = dir * MOVE_SPEED / 2;
	trans->position.x += acc.x;
	trans->position.z += acc.z;

	// ごり押し
	if (trans->position.z >= RIMIT_DIST) {
		trans->position.z = RIMIT_DIST;
	}

	if (trans->position.x >= RIMIT_DIST) {
		trans->position.x = RIMIT_DIST;
	}
	if (trans->position.z <= -RIMIT_DIST) {
		trans->position.z = -RIMIT_DIST;
	}
	if (trans->position.x <= -RIMIT_DIST) {
		trans->position.x = -RIMIT_DIST;
	}

}

void InputMove::MoveRotate(float * rot,bool pls){
	if (*rot < MAX_ROTATE) {
		if(pls) *rot += ACC_ROTATE;
		else *rot -= ACC_ROTATE;
	}
}
