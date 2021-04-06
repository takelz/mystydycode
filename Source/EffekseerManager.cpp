
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
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	manager = Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X�̐���
	renderer = ::EffekseerRendererDX11::Renderer::Create(GetDX11Device(), GetDX11DeviceContext(), 2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// ���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// ������
	//XAudio2Create(&g_xa2);
	//g_xa2->CreateMasteringVoice(&g_xa2_master);

	// ���Đ��p�C���X�^���X�̐���
	//sound = ::EffekseerSound::Sound::Create(xa2, 16, 16);

	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	//manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	//manager->SetSoundLoader(sound->CreateSoundLoader());




}

void EffekseerManager::Draw(){
	
	// �G�t�F�N�g�̈ړ��������s��
	//manager->AddLocation(handle, Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	//manager->Update();
	// ���e�s���ݒ�BNEAR��FAR����ւ����炱����ς���
	renderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_X / (float)SCREEN_Y, 1.0f, 10000.0f));
	
	// �J�����s���ݒ�
	Effekseer::Vector3D  pos = 
		Effekseer::Vector3D(Camera::Instance()->paramPosition.x, Camera::Instance()->paramPosition.y, Camera::Instance()->paramPosition.z);
	Effekseer::Vector3D  look = 
		Effekseer::Vector3D(Camera::Instance()->GetLookat().x, Camera::Instance()->GetLookat().y, Camera::Instance()->GetLookat().z);
	Effekseer::Vector3D  up = 
		Effekseer::Vector3D(Camera::Instance()->GetUp().x, Camera::Instance()->GetUp().y, Camera::Instance()->GetUp().z);
	
	renderer->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(pos, look, up));
	
	
	// �G�t�F�N�g�̕`��J�n�������s���B
	renderer->BeginRendering();
	
	// �G�t�F�N�g�̕`����s���B
	manager->Draw();
	
	// �G�t�F�N�g�̕`��I���������s���B
	renderer->EndRendering();
}


void EffekseerManager::Delete() {
	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	manager->Destroy();

	// ���ɉ��Đ��p�C���X�^���X��j��
	sound->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	renderer->Destroy();

	// XAudio2�̉��
	if (xa2_master != NULL)
	{
		xa2_master->DestroyVoice();
		xa2_master = NULL;
	}
	ES_SAFE_RELEASE(xa2);
}