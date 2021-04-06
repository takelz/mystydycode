#pragma once

#include"DXg11/GameObjectList.h"

	class GameClear :public GameObject {
	public:
		GameClear();
		~GameClear() {};

		void Update();
		void ScreenDraw();


	private:
		int lifeCnt;
		bool pushed;
	};



