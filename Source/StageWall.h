#pragma once

#include <DXg11/GameObjectList.h>

class WallObject :public GameObject {
public:
	WallObject();
	~WallObject() {};

	void Update();
	void Draw();

private:


	RenderModel model;
};




class StageWall :public GameObject {
public:
	StageWall();
	~StageWall() {};

	//void Update();
	//void Draw();

private:



};

