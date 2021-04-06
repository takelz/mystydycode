#pragma once

#include"DXg11/GameObjectList.h"
#include"StartGame.h"


class TitleName :public GameObject {
public:
	TitleName();
	~TitleName() {};

	void Update();
	void ScreenDraw();

private:
	Sprite start;
};


class TitleCenter :public GameObject {
public:
	TitleCenter();
	~TitleCenter() {};

	void Update();
	void Draw();


private:
	void CalcRotateCamera();

	RenderModel crystal;
	Transform crystalPos;


	float angle;
};



