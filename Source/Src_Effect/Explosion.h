#pragma once

#include <DXg11/GameObjectList.h>
#include <EffekseerManager.h>

class Explosion :public GameObject {
public:
	Explosion();
	~Explosion() {};

	void Update();
	void Draw();
	void Play(Vector3 pos);
private:
	Effekseer::Effect	*effect;
	Effekseer::Handle	handle;


	int lifeCnt;
};



