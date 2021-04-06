#pragma once

#include <d3d11.h>
#include <XAudio2.h>
#pragma comment(lib, "d3d11.lib" )
#pragma comment(lib, "xaudio2.lib" )

#include "DXg11/GameObjectList.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Debug.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.lib" )



#define MAX_EFFECT_OBJ (1)

class EffectData {
public:
	Effekseer::Effect *effect;
	Effekseer::Handle	handle;
	Vector3 pos;
};

class EffekseerManager {
public:
	enum Obj{
		SAMPLE,
		OBJ_MAX,
	};

	Vector3 position;
	void Draw();
	void Delete();
	Effekseer::Manager* GetManager() { return manager; }
private:
	static EffekseerManager *instance;

	static Effekseer::Manager			*manager;
	static EffekseerRenderer::Renderer	*renderer;
	static EffekseerSound::Sound		*sound;
	static Effekseer::Effect			*effect[MAX_EFFECT_OBJ];

	static IXAudio2*						g_xa2;
	static IXAudio2MasteringVoice*			g_xa2_master;

	Effekseer::Handle				handle = -1;
	EffekseerManager();
	~EffekseerManager() { Delete(); };

public:
	EffekseerManager(const EffekseerManager&) = delete;
	EffekseerManager& operator=(const EffekseerManager&) = delete;
	EffekseerManager(EffekseerManager&&) = delete;
	EffekseerManager& operator=(EffekseerManager&&) = delete;


	static EffekseerManager* Get() {
		if (instance == nullptr)
			instance = new EffekseerManager();
		return instance;
	}
};