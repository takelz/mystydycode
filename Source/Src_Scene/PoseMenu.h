#pragma once

#include"DXg11/GameObjectList.h"

	class PoseMenu :public GameObject {
	public:
		PoseMenu();
		~PoseMenu() {};

		void Update();
		void Draw();
		void LateUpdate();

	private:
		bool isStop;
	};



