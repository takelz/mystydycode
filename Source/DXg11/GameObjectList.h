#pragma once
#include"GameObjectBase.h"
#include<map>
#include<functional>



#define RENDER_PATH (RenderTargetManager::Get()->GetCurrentPath())

// リストはunityのヒエラルキービューのイメージで

class GameObjectList {
	friend class ColliderCircle;
	friend class ColliderAABB;
	friend class ColliderOBB;
public:

	static GameObjectList *Get() {
		static GameObjectList gameObjectList;
		return &gameObjectList;
	}

	GameObject* Generate(GameObject *obj);
	
	const std::list<GameObject*> *GetTagList(TAG tag);

	// ゲームオブジェクトリストに入れる前にTAGが設定されているなら自動的に割り当てられる
	// リストにいれた後ならばこの関数でタグを設定する
	void RegisterTagObject(GameObject* obj, TAG setTag);

	// 実行準は上から
	void Setup();

	void Start();
	void Collide();			// 
	void Update();
	void Draw();	
	void LateUpdate();		// Drawの直後に更新


	void Uninit();

	void Reset();			// リスト内をすべてリセット(非破壊オブジェクトは除く)
	void CompletelyReset();	// 非破壊オブジェクトを含む完全にリセット、は実装しない

	void AddDrawList(GameObject *obj) { addDrawList.emplace_back(obj); };
	// イテレータ―の初期化で使う
	std::list<std::function<void()> >::iterator* GetItrEnd() { return &itrEnd; }

	// 全オブジェクトのリスト
	std::list<GameObject*> objectList;	
	// タグ分けされたリスト
	std::map<TAG,std::list<GameObject*>> tagList;
	// Start()関数
	std::list<std::function<void()> > startObjectList;

	// 歪み用関数
	std::list<GameObject*> addDrawList;

	std::list<std::function<void()> >::iterator itrEnd;

	// アップデート関数を止める用、LateUpdate()は止まらない
	bool stopUpdate;

	GameObjectList();
	~GameObjectList() {};

};