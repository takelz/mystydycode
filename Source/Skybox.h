#pragma once

#include"DXg11/GameObjectList.h"

class Skybox :public GameObject{
public:
	Skybox();
	~Skybox();

	void Start() {};
	void Uninit() {};
	void Update();
	void Draw();


private:

};