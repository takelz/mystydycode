// 地面クラス実装 (Land.cpp)
#include "Land.h"

using namespace DirectX;

const char* PATH_LAND_MODEL = "Assets/land.fbx";

const float LAND_POS_X = 0.0f;
const float LAND_POS_Y = 0.0f;
const float LAND_POS_Z = 0.0f;

// コンストラクタ
Land::Land(){
	AddComponent(COMPONENT_MODEL);
	GetModel()->Load(PATH_LAND_MODEL);
	transform.scale = 5.0f;
	transform.position.y = 0.0f;

}
// デストラクタ
Land::~Land(){
}


void Land::Update() {

}

void Land::Draw() {

	GetModel()->Draw(transform);
}

