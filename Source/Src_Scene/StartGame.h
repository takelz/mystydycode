#pragma once

#include"DXg11/GameObjectList.h"
#include<Fade.h>

	class StartGame :public GameObject {
	public:
		StartGame();
		~StartGame() {};

		void Update();
		void Draw();


	private:

		void Push();

		bool pushed;
	};



