#pragma once
#include"DXg11/GameObjectList.h"


class Number :public GameObject {
public:

	Number();
	~Number() {};

	void Update();
	void ScreenDraw();

	
	Vector3 pos;
	Vector3 scale;
	int num;
	int maxDigit;	// Œ…”
private:
	Sprite sprite[10];



};




class UI_Number {
public:
	UI_Number();
	~UI_Number();

	void Write(Vector3 pos, Vector3 scale, int num) {
		numObj->pos = pos;
		numObj->scale = scale;
		numObj->num = num;
		numObj->maxDigit = (int)log10f((float)num) + 1;	// log10‚ÅŒ…”‚ğ‹‚ß‚é
	}

private:
	Number *numObj;

};
