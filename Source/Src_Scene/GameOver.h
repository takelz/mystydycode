#pragma once

#include"DXg11/GameObjectList.h"

	class GameOver :public GameObject {
	public:
		GameOver();
		~GameOver() {};

		void Update();
		void ScreenDraw();


	private:
		int lifeCnt;
	};



