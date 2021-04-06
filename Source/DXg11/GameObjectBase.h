#pragma once


#include <list>
#include <string>
#include <functional>

#include <Model/ModelObject.h>
#include <DirectX/DX11Util.h>
#include <DirectX/DX11Graphics.h>
#include <DirectX/DX11Light.h>
#include <DirectX/DrawObject.h>

#include "Transform.h"
#include "RenderModel.h"
#include "Collider.h"
#include "Input.h"
#include "Sprite.h"
#include "Plane.h"
#include "PlaneSimple.h"
#include "Mathf.h"
#include "RenderTargetManager.h"


// �e�q�֌W�ɂ��āA�e���q������ɐ������ꂽ�I�u�W�F�N�g���Ƃ��܂��쓮���Ȃ��\��������

#define PUSH  &0x8000
#define		SCREEN_X		(1200)
#define		SCREEN_Y		(750)

// �_�C�i�~�b�N�L���X�g(�R���C�_����̂ݕK���g�p�A����ȊO�̎g�p�͋֎~)
#define D_CAST(class,obj) dynamic_cast<class*>(obj)

#define MSDBG 		MessageBox(NULL, "", "", MB_OK);


using namespace DirectX;

typedef enum {
	TAG_NONE = 0,
	TAG_ENEMY,
	TAG_PLAYER,
	TAG_PROTECED_OBJ,
	TAG_UNIT,
	TAG_MAX,
}TAG;


class GameObject {
	friend class GameObjectList;
	// �����蔻��ɂ�private�̏����Q�Ƃł���悤��
	friend class ColliderCircle;
	friend class ColliderAABB;
	friend class ColliderOBB;
public:
	typedef enum {
		COMPONENT_MODEL,
		COMPONENT_COLLIDER_CIRCLE,
		COMPONENT_COLLIDER_AABB,
		COMPONENT_COLLIDER_OBB,
		COMPONENT_COLLIDER_RAY,
		COMPONENT_SPRITE,
		COMPONENT_PLANE,
		COMPONENT_PLANE_SIMPLE,


	}COMPONENT;

	GameObject();
	GameObject(Transform setTransform);

	virtual ~GameObject();

	// �R���X�g���N�^��Awake�BStart��Awake�̎��ɌĂяo����� 
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate() {};
	virtual void Draw();
	virtual void LateDraw();	// LateUpdate�̌�ɕ`��
	virtual void LastDraw() {};
	virtual void ScreenDraw() {};	// (���UI�p)


	// �R���|�[�l���g�p�B���ʂ�Update���x��Update�B�ꉞ���[�U���g����悤�ɁB
	void SystemLateUpdate();


	// �j��
	void Destroy() { isDestroy = true; };
	void DontDestroy() { dontDestroy = true; }
	void EnableDestroy() { dontDestroy = false; }
	// �^�O�̎擾
	TAG Tag() { return tag; };

	// �{���͐e�q�֌W�͂��̃N���X�ɂ��ꂽ��
	Transform transform;

	// ���̑��X�e�[�^�X
	std::string name;
	bool active;		// Update�L����

protected:
	void SetTag(TAG tag);
	
	// ���̊֐��͕K��Start���R���X�g���N�^�̒��ŌĂяo��
	void AddComponent(COMPONENT component);


	// �R���|�[�l���g
	RenderModel*	GetModel();
	ColliderBase*	GetCollider();
	Sprite*			GetSprite();
	Plane*			GetPlane();
	PlaneSimple*	GetPlaneSimple();
private:
	// �����̐e�I�u�W�F�N�g
	GameObject *parent;

	// �R���|�[�l���g�Q
	RenderModel		*model;
	ColliderBase	*pCollider;
	ColliderCircle	*colliderCircle;
	ColliderAABB	*colliderAABB;
	ColliderOBB		*colliderOBB;
	Sprite			*sprite;
	Plane			*plane;	
	PlaneSimple		*planeSimple;


	bool	isDestroy;	
	bool	dontDestroy;
	bool	endStart;		// Start�֐��ĂяI��������
	TAG		tag;			// �^�O�̓I�u�W�F�N�g������O�ɐݒ�
	std::list<GameObject*>::iterator tagListItr;				// ���g���Ԃ牺�����Ă�^�O���X�g�̃C�e���[�^��ێ�
	std::list<std::function<void()> >::iterator	startListItr;	// �X�^�[�g�֐��̃C�e���[�^
	std::list<std::function<void()> >::iterator	collideListItr;	// �����蔻��֐��̃C�e���[�^




};