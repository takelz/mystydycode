/**
 * @brief 入力一元管理
 * @author ryota.ssk
 * @note	とりあえず一人用として入力を考える
 *			あとはあまり他のファイルに依存しないようにしたり
 */
#ifndef __INPUT_INTERFACE_H__
#define __INPUT_INTERFACE_H__

#include <windows.h>

class InputInterface
{
	//----------
	// 構造体、列挙型
	//----------
public:
	// ボタン
	enum ButtonKind
	{
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,

		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,

		BUTTON_L1,
		BUTTON_L2,
		BUTTON_L3,
		BUTTON_R1,
		BUTTON_R2,
		BUTTON_R3,

		BUTTON_START,
		BUTTON_SELECT,

		BUTTON_MAX
	};
	// 軸
	enum AxisKind
	{
		AXIS_LX,	// 左スティックX軸
		AXIS_LY,	// 左スティックY軸
		AXIS_LZ,	// L2ボタン押し込み
		AXIS_RX,	// 右スティックX軸
		AXIS_RY,	// 右スティックY軸
		AXIS_RZ,	// R2ボタン押し込み

		AXIS_MAX
	};
	// 周辺機器
	enum EquipmentKind
	{
		EQUIPMENT_KEYBOARD,
		EQUIPMENT_GAMEPAD,

		EQUIPMENT_MAX
	};

	//----------
	// 関数
	//----------
public:
	InputInterface();
	~InputInterface();

	void Update();
	void Draw();

	void SetRepeat(int wait, int tick);


	bool IsPress(ButtonKind button) const;
	bool IsTrigger(ButtonKind button) const;
	bool IsRelease(ButtonKind button) const;
	bool IsRepeat(ButtonKind button) const;

	float GetAxis(AxisKind axis) const;

private:
	bool m_oldButton[BUTTON_MAX];
	bool m_nowButton[BUTTON_MAX];
	float m_axis[AXIS_MAX];
	int m_repeatButtonCount[BUTTON_MAX];
	int m_repeatWait;
	int m_repeatTick;

	int m_buttonKeyConfig[BUTTON_MAX];
	int m_buttonPadConfig[BUTTON_MAX];
	int m_axisKeyConfig[AXIS_MAX][2];
	int m_axisPadConfig[AXIS_MAX];
};

#endif // __INPUT_INTERFACE_H__