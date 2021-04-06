
#include"StageWall.h"
#include<DirectX/DrawObject.h>



using namespace DirectX;



// À•W0,0,0‚É’u‚­

#define LIFE_TIME (20)

WallObject::WallObject() {

	model.Load("Assets/wall/newWall.fbx");
}	

void WallObject::Update() {

}

void WallObject::Draw() {
	model.Draw(transform);
}

StageWall::StageWall(){
	GameObject *obj = new WallObject();
	obj->transform.position.x = 3500.0f;
	obj->transform.scale = 300.0f;
	obj->transform.scale.z *= 6.0f;
	GameObjectList::Get()->Generate(obj);
	
	obj = new WallObject();
	obj->transform.position.x = -3500.0f;
	obj->transform.scale = 300.0f;
	obj->transform.scale.z *= 6.0f;
	GameObjectList::Get()->Generate(obj);

	obj = new WallObject();
	obj->transform.position.z = 3500.0f;
	obj->transform.scale = 300.0f;
	obj->transform.scale.x *= 6.0f;
	obj->transform.rotation.y = 90.0f;
	GameObjectList::Get()->Generate(obj);

	obj = new WallObject();
	obj->transform.position.z = -3500.0f;
	obj->transform.scale = 300.0f;
	obj->transform.scale.x *= 6.0f;
	obj->transform.rotation.y = 90.0f;
	GameObjectList::Get()->Generate(obj);

}
