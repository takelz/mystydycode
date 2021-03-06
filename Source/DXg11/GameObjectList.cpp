#include"GameObjectList.h"
#include"../Camera.h"
#include <Shader/ShadowWrite.h>
#include <Shader/NormalRender.h>
#include <Shader/PostBlur.h>
#include <Shader/PostBloom.h>
#include <Shader/DepthBuffer.h>
#include <Shader/PostFog.h>
#include <Shader/PostNoiseWave.h>
#include <Shader/PostDistortionObject.h>
#include <Shader/PostPerlinGlass.h>

static unsigned int currentUniqueNum = 0;


GameObjectList::GameObjectList(){
	itrEnd = startObjectList.end();

	// テスト
	ShadowWrite::Get()->Create(0,0);
}

void GameObjectList::Setup() {
	// タグのデータリストの準備
	for (int i = 0; i < TAG_MAX;i++) {
		tagList[(TAG)i];
	}
}

void GameObjectList::Start() {
	if (!startObjectList.size())return;	// 何もないならスルー
	
	auto itr = startObjectList.begin();
	int size = startObjectList.size();
	for (int i = 0; i < size; i++) {
		(*itr)();
		++itr;
	}
	startObjectList.clear();
}


void GameObjectList::Collide() {

}


void GameObjectList::Update() {
	// シェーダーのtime用
	ShaderTimeCountUp();

	auto itr = objectList.begin();
	auto endPoint = objectList.end();
	while (itr != endPoint) {
		// スタート呼ばれてないときはスタート
		if (!(*itr)->endStart) {
			(*itr)->Start();
			(*itr)->endStart = true;
		}
		if(!stopUpdate)
			(*itr)->Update();
		// 削除
		if ((*itr)->isDestroy) {
			// タグリストから削除
			if((*itr)->tag != TAG_NONE)
				tagList[(*itr)->tag].erase((*itr)->tagListItr);

			// Startリストから削除
			//if ((*itr)->startListItr != GetItrEnd()) {
			//
			//}

			// オブジェクトリストから削除
			GameObject *obj = (*itr);	// こっちに移してからイテレータ使ってリスト削除
			itr = objectList.erase(itr);
			delete obj;
		}
		if (itr == endPoint) break;
		++itr;
	}
}

const std::list<GameObject*> *GameObjectList::GetTagList(TAG tag) {
	return &tagList[tag];
}

void GameObjectList::RegisterTagObject(GameObject* obj, TAG setTag) {
	if (obj->tag != TAG_NONE) {
		MessageBox(NULL, "すでにタグが設定されています", "GameObjectError", MB_OK);
		return;
	}
	if (setTag == TAG_NONE)return;	// このタグはうけつけない　

	tagList[obj->tag].emplace_back(obj);
	obj->tagListItr = --tagList[obj->tag].end();	// 最後のendより一つ前がリストの最後のデータ

}




void GameObjectList::Draw() {



	auto itr = objectList.begin();
	auto endPoint = objectList.end();


	for (int loop = 1; loop <= MAX_RENDERTARGET_PATH; loop++) {
		RenderTargetManager::Get()->OrderRender(loop);

		itr = objectList.begin();
		endPoint = objectList.end();


		// ここを組み替える時間がないので手動でシェーダ切り替え
		while (itr != endPoint) {
			if (RENDER_PATH == 1) {	// 一度目のDEPTHWRITEは光源からのもの
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_WRITE);
			}
			else if (RENDER_PATH == 2) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_BUFFER);
			}
			else if (RENDER_PATH == 3 || RENDER_PATH == 4) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_SHADOW);
			}

			(*itr)->Draw();
			++itr;
		}

		itr = objectList.begin();
		endPoint = objectList.end();
		while (itr != endPoint) {
			if (RENDER_PATH == 1) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_WRITE);
			}
			else if (RENDER_PATH == 2) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_DEPTH_BUFFER);
			}
			else if (RENDER_PATH == 3 || RENDER_PATH == 4) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_SHADOW);
			}
			(*itr)->LateDraw();
			++itr;
		}

		if (RENDER_PATH == 2) {
			RenderTargetManager::Get()->OrderRender(5);
			auto addItr = addDrawList.begin();
			auto  addEndPoint = addDrawList.end();
			while (addItr != addEndPoint) {
				ShaderSelector::Get()->SelectModelShaderRender(SHADER_NORMAL);
				(*addItr)->LateDraw();
				++addItr;
			}
		}

	}


}




void GameObjectList::LateUpdate() {
	auto itr = objectList.begin();
	auto endPoint = objectList.end();


	while (itr != endPoint) {
		(*itr)->SystemLateUpdate();
		++itr;
	}



	// ここにレンダー結果の表示


	// ターゲット先を変える
	GetDX11Graphics()->EnableZBuffer(false);

	//Blur::Get()->Render();
	PostBloom::Get()->Render();
	PostFog::Get()->Render();
	PostNoiseWave::Get()->Render();
	PostPerlinGlass::Get()->Render();


	// バックバッファ、スクリーン座標上に描画する
	GetDX11Graphics()->EnableZBuffer(false);
	NormalRender::Get()->Render();		
	//ShadowWrite::Get()->Render();
	//DepthBuffer::Get()->Render();
	//PostDistortionObject::Get()->Render();

	itr = objectList.begin();
	endPoint = objectList.end();
	while (itr != endPoint) {
		(*itr)->ScreenDraw();
		++itr;
	}


	GetDX11Graphics()->EnableZBuffer(true);
}





void GameObjectList::Uninit() {

}

GameObject* GameObjectList::Generate(GameObject *obj) {
	obj->transform.instanceID = currentUniqueNum;
	currentUniqueNum++;

	objectList.emplace_back(obj);
	if (obj->tag != TAG_NONE) {
		tagList[obj->tag].emplace_back(obj);
		obj->tagListItr = --tagList[obj->tag].end();	// 最後のendより一つ前がリストの最後のデータ
	}
	return obj;
}

void GameObjectList::Reset() {
	auto itr = objectList.begin();
	auto endPoint = objectList.end();
	
	// 追加のリストから削除
	addDrawList.clear();


	while (itr != endPoint) {
		// 非破壊オブジェクトでなければ削除
		if (!(*itr)->dontDestroy) {
			// タグリストから削除。タグのイテレータは各オブジェクトが保持してるのでそれを使う
			if ((*itr)->tag != TAG_NONE)
				tagList[(*itr)->tag].erase((*itr)->tagListItr);
			// オブジェクトリストから削除
			GameObject *obj = (*itr);	// こっちに移してからイテレータ使ってリスト削除
			itr = objectList.erase(itr);
			delete obj;
		}
		else {
			++itr;
		}

		if (itr == endPoint)break;
		
	}
}