
#include"ProtectedObject.h"
#include<DirectX/DrawObject.h>
#include"../StageManager.h"
#include"../EffekseerManager.h"
#include"../Camera.h"
#include<Shader/PostNoiseWave.h>


#define DEFAULT_LIFE (15)
#define OFFSET_Y (50.0f)

using namespace DirectX;


#define UI_POS_X (SCREEN_X/2)
#define UI_POS_Y (22)

// 座標0,0,0に置く



ProtectedObject::ProtectedObject() {
	AddComponent(COMPONENT_MODEL);

	GetModel()->Load("Assets/crystal2/crystal_17_2.fbx");


	AddComponent(COMPONENT_COLLIDER_OBB);
	transform.scale = 30;

	SetTag(TAG_PROTECED_OBJ);
	//transform.rotation.x = -90;
	transform.position.y = OFFSET_Y;
	transform.scale = 80.0f;

	GetCollider()->SetupStatus(GetModel()->GetCenter(), &transform,
		1, GetModel()->GetBBox(), &transform.GetMatrix());

	life = DEFAULT_LIFE;

	animator.Play(0, true);

	
	lifeUI.SetMaxLife(DEFAULT_LIFE, Vector3(UI_POS_X, UI_POS_Y));



	barrier = new Barrier();
	GameObjectList::Get()->Generate(barrier);
}

void ProtectedObject::Start() {

}

void ProtectedObject::Update() {
	GetCollider()->mtx = &transform.GetMatrix();
	GetCollider()->center = (GetModel()->GetCenter());
	

	//hpNum.Write(Vector3(50, 50), Vector3(50, 50, 1), life);

	lifeUI.SetCurrentLife(life);
	
	barrier->transform.position = transform.position;
	barrier->isOn = isBarrier;
	EndGame();
}

void ProtectedObject::Draw() {
	if (RENDER_PATH == 3) {
		ShaderSelector::Get()->SelectModelShader(SHADER_CRYSTAL);

		ShaderSelector::Get()->SetConstantBuffers()->
			PSSetConstantBuffers(2, 1, Camera::Instance()->GetBuffer());
	}

	GetModel()->Draw(transform);


}

void ProtectedObject::ScreenDraw(){

}

void ProtectedObject::AddDamage() {
	static bool muteki = false;	// デバッグ用
	if (Input::GetKey(DIK_6, INPUT::TRIGGER)) {
		muteki = !muteki;
		MessageBox(NULL, "クリスタルを無敵にしました", "デバッグ", MB_OK);
	}
	if (muteki) return;
	
	if (!isBarrier) {
		life--;
		PostNoiseWave::Get()->TriggerOn();
	} else {
		isBarrier = false;
	}
}

void ProtectedObject::EndGame() {
	if (life <= 0) {
		// ここにゲーム終了処理
		StageManager::Get()->GameOver();
	}
}
