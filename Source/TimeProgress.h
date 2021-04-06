#pragma once

#include<DXg11/GameObjectList.h>

// ƒV[ƒ“‚Ì“Ç‚İ‚İ‚É¶¬‚·‚é
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



