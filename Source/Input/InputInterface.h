/**
 * @brief ���͈ꌳ�Ǘ�
 * @author ryota.ssk
 * @note	�Ƃ肠������l�p�Ƃ��ē��͂��l����
 *			���Ƃ͂��܂葼�̃t�@�C���Ɉˑ����Ȃ��悤�ɂ�����
 */
#ifndef __INPUT_INTERFACE_H__
#define __INPUT_INTERFACE_H__

#include <windows.h>

class InputInterface
{
	//----------
	// �\���́A�񋓌^
	//----------
public:
	// �{�^��
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
	// ��
	enum AxisKind
	{
		AXIS_LX,	// ���X�e�B�b�NX��
		AXIS_LY,	// ���X�e�B�b�NY��
		AXIS_LZ,	// L2�{�^����������
		AXIS_RX,	// �E�X�e�B�b�NX��
		AXIS_RY,	// �E�X�e�B�b�NY��
		AXIS_RZ,	// R2�{�^����������

		AXIS_MAX
	};
	// ���Ӌ@��
	enum EquipmentKind
	{
		EQUIPMENT_KEYBOARD,
		EQUIPMENT_GAMEPAD,

		EQUIPMENT_MAX
	};

	//----------
	// �֐�
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