#pragma once

#include<DXg11/GameObjectList.h>

// シーンの読み込み時に生成する
class Time :public GameObject{
public:
	Time();
	~Time();

	void Update();
	void Draw();
	static void Reset();

	static int GetCount() { return timeCount; };
private:

	static int timeCount;
};



