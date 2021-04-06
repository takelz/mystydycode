#pragma once

#include"DXg11/GameObjectList.h"

	class WaveUI :public GameObject {
	public:
		WaveUI(const char * name);
		~WaveUI() {};

		void Update();
		void ScreenDraw();
		void LateUpdate();

	private:
		float alpha;
		float posX;
	};



