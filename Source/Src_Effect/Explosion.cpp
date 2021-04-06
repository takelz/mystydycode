
#include"Explosion.h"
#include<DirectX/DrawObject.h>
#include"../StageManager.h"
#include"../EffekseerManager.h"


using namespace DirectX;



// À•W0,0,0‚É’u‚­

#define LIFE_TIME (40)

Explosion::Explosion() {


	effect = Effekseer::Effect::Create(EffekseerManager::Get()->GetManager(), (const EFK_CHAR*)L"Effect/boom/boom.efk");
	if (effect == NULL) MSDBG


	lifeCnt = 0;
}	

void Explosion::Update() {
	if (LIFE_TIME < lifeCnt) {
		ES_SAFE_RELEASE(effect);
		Destroy();
	}
	lifeCnt++;
}

void Explosion::Draw() {

}

void Explosion::Play(Vector3 pos){
	handle = EffekseerManager::Get()->GetManager()->Play(effect, pos.x, pos.y, pos.z);
	EffekseerManager::Get()->GetManager()->SetScale(handle, 15.0f, 15.0f, 15.0f);
}

