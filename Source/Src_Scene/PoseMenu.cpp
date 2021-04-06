
#include"PoseMenu.h"
#include<Shader/PostPerlinGlass.h>

PoseMenu::PoseMenu() {
	isStop = false;
}

void PoseMenu::Update() {

}

void PoseMenu::Draw() {


}

void PoseMenu::LateUpdate() {
	if (Input::GetKey(DIK_P, INPUT::TRIGGER) || DIGetPadTrigger(PAD_START)) {
		isStop = !isStop;
	}
	GameObjectList::Get()->stopUpdate = isStop;
	PostPerlinGlass::Get()->renderGlassFilter = isStop;
}