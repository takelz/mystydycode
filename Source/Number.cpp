#include"Number.h"


UI_Number::UI_Number() {
	numObj = new Number();
	GameObjectList::Get()->Generate(numObj);
}


UI_Number::~UI_Number() {
	numObj->Destroy();
	numObj = nullptr;
}



Number::Number() {
	sprite[0].Load("Assets/Textures/Number/00.png");
	sprite[1].Load("Assets/Textures/Number/01.png");
	sprite[2].Load("Assets/Textures/Number/02.png");
	sprite[3].Load("Assets/Textures/Number/03.png");
	sprite[4].Load("Assets/Textures/Number/04.png");
	sprite[5].Load("Assets/Textures/Number/05.png");
	sprite[6].Load("Assets/Textures/Number/06.png");
	sprite[7].Load("Assets/Textures/Number/07.png");
	sprite[8].Load("Assets/Textures/Number/08.png");
	sprite[9].Load("Assets/Textures/Number/09.png");

}

void Number::Update() {

}

void Number::ScreenDraw() {
	int calcDigid = 10;
	int digidNumber;
	for (int i = 0; i < maxDigit; i++) {
		digidNumber = num % calcDigid;	// 毎ループ、次の桁を取り出すためにcalcDigitは10倍していく
		if(i >= 1)
			digidNumber = digidNumber / (calcDigid / 10 );
		
		// Xは右から数を置いていく計算
		sprite[digidNumber].pos = Vector3((pos.x + scale.x * maxDigit) - scale.x * i,pos.y,0.0f);
		sprite[digidNumber].scale = scale;
		sprite[digidNumber].Render();

		calcDigid *= 10;
	}
}

/*	//メモ
int k = 0;
for (int i = 0; i < MAX_DIGITS && i < str.size(); i++) {
	k = str[i] - '0';
	if (k < 0 || k > 9) continue;
	number[i][k]->SetPosition({ ALL_UI_POS_X + i * NUM_OFFSET_X,NUM_START_POS_Y + count * 120 });
	number[i][k]->SetAlpha(1.0f);
}

		//-----それぞれの桁の取り出し
		nFirstDigit = num % 10;
		nSecondDigit = num / 10 % 10;
		nThirdDigit = num / 100 % 10;
		nFourthDigit = num / 1000 % 10;

*/