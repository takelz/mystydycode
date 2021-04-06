
#include"TimeProgress.h"
#include<DirectX/DrawObject.h>

using namespace DirectX;

int Time::timeCount;


Time::Time() {

}


Time::~Time() {
}

void Time::Update() {
	timeCount++;
}

void Time::Draw() {

}

void Time::Reset() {
	timeCount = 0;
}
