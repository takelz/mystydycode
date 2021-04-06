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


// 親子関係について、親が子よりも後に生成されたオブジェクトだとうまく作動しない可能性がある

#define PUSH  &0x8000
#define		SCREEN_X		(1200)
#define		SCREEN_Y		(750)

// ダイナミックキャスト(コライダ判定のみ必ず使用、それ以外の使用は禁止)
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
	// 当たり判定にはprivateの情報を参照できるように
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

	// コンストラクタがAwake。StartはAwakeの次に呼び出される 
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate() {};
	virtual void Draw();
	virtual void LateDraw();	// LateUpdateの後に描画
	virtual void LastDraw() {};
	virtual void ScreenDraw() {};	// (主にUI用)


	// コンポーネント用。普通のUpdateより遅いUpdate。一応ユーザも使えるように。
	void SystemLateUpdate();


	// 破壊
	void Destroy() { isDestroy = true; };
	void DontDestroy() { dontDestroy = true; }
	void EnableDestroy() { dontDestroy = false; }
	// タグの取得
	TAG Tag() { return tag; };

	// 本当は親子関係はこのクラスにいれたい
	Transform transform;

	// その他ステータス
	std::string name;
	bool active;		// Update有効か

protected:
	void SetTag(TAG tag);
	
	// この関数は必ずStartかコンストラクタの中で呼び出す
	void AddComponent(COMPONENT component);


	// コンポーネント
	RenderModel*	GetModel();
	ColliderBase*	GetCollider();
	Sprite*			GetSprite();
	Plane*			GetPlane();
	PlaneSimple*	GetPlaneSimple();
private:
	// 自分の親オブジェクト
	GameObject *parent;

	// コンポーネント群
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
	bool	endStart;		// Start関数呼び終わったやつ
	TAG		tag;			// タグはオブジェクトを入れる前に設定
	std::list<GameObject*>::iterator tagListItr;				// 自身がぶら下がってるタグリストのイテレータを保持
	std::list<std::function<void()> >::iterator	startListItr;	// スタート関数のイテレータ
	std::list<std::function<void()> >::iterator	collideListItr;	// 当たり判定関数のイテレータ




};