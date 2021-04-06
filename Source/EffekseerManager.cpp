
#include "EffekseerManager.h"
#include "Camera.h"


EffekseerManager* EffekseerManager::instance = nullptr;

Effekseer::Manager*				EffekseerManager::manager;
EffekseerRenderer::Renderer*	EffekseerManager::renderer;
EffekseerSound::Sound*			EffekseerManager::sound;
Effekseer::Effect*				EffekseerManager::effect[MAX_EFFECT_OBJ];

IXAudio2*						xa2;
IXAudio2MasteringVoice*			xa2_master;

EffekseerManager::EffekseerManager() {
	// エフェクト管理用インスタンスの生成
	manager = Effekseer::Manager::Create(2000);

	// 描画用インスタンスの生成
	renderer = ::EffekseerRendererDX11::Renderer::Create(GetDX11Device(), GetDX11DeviceContext(), 2000);

	// 描画用インスタンスから描画機能を設定
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// 音生成
	//XAudio2Create(&g_xa2);
	//g_xa2->CreateMasteringVoice(&g_xa2_master);

	// 音再生用インスタンスの生成
	//sound = ::EffekseerSound::Sound::Create(xa2, 16, 16);

	// 音再生用インスタンスから再生機能を指定
	//manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// 音再生用インスタンスからサウンドデータの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	//manager->SetSoundLoader(sound->CreateSoundLoader());




}

void EffekseerManager::Draw(){
	
	// エフェクトの移動処理を行う
	//manager->AddLocation(handle, Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	//manager->Update();
	// 投影行列を設定。NEARやFAR入れ替えたらここを変える
	renderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_X / (float)SCREEN_Y, 1.0f, 10000.0f));
	
	// カメラ行列を設定
	Effekseer::Vector3D  pos = 
		Effekseer::Vector3D(Camera::Instance()->paramPosition.x, Camera::Instance()->paramPosition.y, Camera::Instance()->paramPosition.z);
	Effekseer::Vector3D  look = 
		Effekseer::Vector3D(Camera::Instance()->GetLookat().x, Camera::Instance()->GetLookat().y, Camera::Instance()->GetLookat().z);
	Effekseer::Vector3D  up = 
		Effekseer::Vector3D(Camera::Instance()->GetUp().x, Camera::Instance()->GetUp().y, Camera::Instance()->GetUp().z);
	
	renderer->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(pos, look, up));
	
	
	// エフェクトの描画開始処理を行う。
	renderer->BeginRendering();
	
	// エフェクトの描画を行う。
	manager->Draw();
	
	// エフェクトの描画終了処理を行う。
	renderer->EndRendering();
}


void EffekseerManager::Delete() {
	// 先にエフェクト管理用インスタンスを破棄
	manager->Destroy();

	// 次に音再生用インスタンスを破棄
	sound->Destroy();

	// 次に描画用インスタンスを破棄
	renderer->Destroy();

	// XAudio2の解放
	if (xa2_master != NULL)
	{
		xa2_master->DestroyVoice();
		xa2_master = NULL;
	}
	ES_SAFE_RELEASE(xa2);
}