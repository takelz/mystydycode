#include"GameObjectBase.h"
#include"GameObjectList.h"
//#include"../Camera.h"

using namespace DirectX;

static unsigned int setInstanceID = 0;

GameObject::GameObject() {
	model = nullptr;
	colliderCircle = nullptr;
	colliderAABB = nullptr;
	colliderOBB = nullptr;
	sprite = nullptr;
	plane = nullptr;
	planeSimple = nullptr;

	parent = nullptr;

	isDestroy = false;
	dontDestroy = false;

	tag = TAG_NONE;
	name = "";
	active = true;
	endStart = false;

	transform.instanceID = setInstanceID;
	setInstanceID++;

	

}

GameObject::GameObject(Transform setTransform) {
	model = nullptr;
	colliderCircle = nullptr;
	colliderAABB = nullptr;

	parent = nullptr;
	isDestroy = false;
	tag = TAG_NONE;
	name = "";
	active = true;

	transform.instanceID = setInstanceID;
	setInstanceID++;
	transform = setTransform;
}

GameObject::~GameObject() {

}


void GameObject::Start() {

}


void GameObject::Update() {

}


void GameObject::Draw() {

}



void GameObject::SystemLateUpdate() {


	LateUpdate();
}

void GameObject::LateDraw() {

}

void GameObject::SetTag(TAG tagName) {
	GameObjectList::Get()->RegisterTagObject(this, tag);
	tag = tagName;
}

void GameObject::AddComponent(COMPONENT component) {
	switch (component){
		case COMPONENT_MODEL:
			if(!model) model = new RenderModel();
			break;
		case COMPONENT_COLLIDER_CIRCLE:
			if(!colliderCircle) pCollider = new ColliderCircle();
			break;
		case COMPONENT_COLLIDER_AABB:
			if(!colliderAABB) pCollider = new ColliderAABB();
			break;
		case COMPONENT_COLLIDER_OBB:
			if (!colliderOBB) pCollider = new ColliderOBB();
			break;
		case COMPONENT_SPRITE:
			if(!sprite) sprite = new Sprite();
			break;
		case COMPONENT_PLANE:
			if (!plane) plane = new Plane();
			break;
		case COMPONENT_PLANE_SIMPLE:
			if (!planeSimple) planeSimple = new PlaneSimple();
			break;

	}
}

//#define NG_COMPONENT_ASSERT(a) _ASSERT(a); MessageBox(NULL, std::to_string().c_str(), "コンポーネントエラー", MB_OK);

RenderModel* GameObject::GetModel() {
	if (model) return model;
	else {
		_ASSERT(model);
		return nullptr;
	}
}

ColliderBase* GameObject::GetCollider() {
	if (pCollider) {
		if (!pCollider->completeSetup) {	// 何か対策を...
			//MessageBox(NULL, 
			//	"AddComponent()の直後にコライダの値がセットされていません\n \
			//	 SetupStatus()を使って初期化してください。","ColliderError", MB_OK);
		}
		return pCollider;
	}
	else {
		_ASSERT(pCollider);
		return nullptr;
	}
}




Sprite* GameObject::GetSprite() {
	if (sprite) return sprite;
	else {
		_ASSERT(sprite);
		return nullptr;
	}
}

Plane* GameObject::GetPlane() {
	if (plane) return plane;
	else {
		_ASSERT(plane);
		return nullptr;
	}
}


PlaneSimple* GameObject::GetPlaneSimple() {
	if (planeSimple) return planeSimple;
	else {
		_ASSERT(planeSimple);
		return nullptr;
	}
}