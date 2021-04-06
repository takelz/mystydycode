#include "InputInterface.h"
#include "InputKeyboard.h"
#include "InputGamePad.h"

InputInterface::InputInterface()
	: m_repeatWait(15)
	, m_repeatTick(2)
{
	for (int i = 0; i < BUTTON_MAX; ++i) {
		m_oldButton[i] = m_nowButton[i] = false;
	}

	// キーボード入力
	m_buttonKeyConfig[BUTTON_A] = VK_Z;
	m_buttonKeyConfig[BUTTON_B] = VK_X;
	m_buttonKeyConfig[BUTTON_X] = VK_C;
	m_buttonKeyConfig[BUTTON_Y] = VK_V;
	m_buttonKeyConfig[BUTTON_L1] = VK_SHIFT;
	m_buttonKeyConfig[BUTTON_L2] = VK_1;
	m_buttonKeyConfig[BUTTON_L3] = VK_2;
	m_buttonKeyConfig[BUTTON_R1] = VK_CONTROL;
	m_buttonKeyConfig[BUTTON_R2] = VK_3;
	m_buttonKeyConfig[BUTTON_R3] = VK_4;
	m_buttonKeyConfig[BUTTON_UP] = VK_UP;
	m_buttonKeyConfig[BUTTON_DOWN] = VK_DOWN;
	m_buttonKeyConfig[BUTTON_LEFT] = VK_LEFT;
	m_buttonKeyConfig[BUTTON_RIGHT] = VK_RIGHT;
	m_buttonKeyConfig[BUTTON_START] = VK_RETURN;
	m_buttonKeyConfig[BUTTON_SELECT] = VK_ESCAPE;
	m_axisKeyConfig[AXIS_LX][0] = VK_A;
	m_axisKeyConfig[AXIS_LX][1] = VK_D;
	m_axisKeyConfig[AXIS_LY][0] = VK_W;
	m_axisKeyConfig[AXIS_LY][1] = VK_S;
	m_axisKeyConfig[AXIS_LZ][0] = VK_Q;
	m_axisKeyConfig[AXIS_LZ][1] = VK_E;
	m_axisKeyConfig[AXIS_RX][0] = VK_J;
	m_axisKeyConfig[AXIS_RX][1] = VK_L;
	m_axisKeyConfig[AXIS_RY][0] = VK_I;
	m_axisKeyConfig[AXIS_RY][1] = VK_K;
	m_axisKeyConfig[AXIS_RZ][0] = VK_U;
	m_axisKeyConfig[AXIS_RZ][1] = VK_O;

	// ゲームパッド入力
	m_buttonPadConfig[BUTTON_A] = PAD_BUTTON_A;
	m_buttonPadConfig[BUTTON_B] = PAD_BUTTON_B;
	m_buttonPadConfig[BUTTON_X] = PAD_BUTTON_X;
	m_buttonPadConfig[BUTTON_Y] = PAD_BUTTON_Y;
	m_buttonPadConfig[BUTTON_L1] = PAD_BUTTON_L1;
	m_buttonPadConfig[BUTTON_L2] = PAD_BUTTON_L2;
	m_buttonPadConfig[BUTTON_L3] = PAD_BUTTON_L3;
	m_buttonPadConfig[BUTTON_R1] = PAD_BUTTON_R1;
	m_buttonPadConfig[BUTTON_R2] = PAD_BUTTON_R2;
	m_buttonPadConfig[BUTTON_R3] = PAD_BUTTON_R3;
	m_buttonPadConfig[BUTTON_UP] = PAD_BUTTON_UP;
	m_buttonPadConfig[BUTTON_DOWN] = PAD_BUTTON_DOWN;
	m_buttonPadConfig[BUTTON_LEFT] = PAD_BUTTON_LEFT;
	m_buttonPadConfig[BUTTON_RIGHT] = PAD_BUTTON_RIGHT;
	m_buttonPadConfig[BUTTON_START] = PAD_BUTTON_START;
	m_buttonPadConfig[BUTTON_SELECT] = PAD_BUTTON_SELECT;
	m_axisPadConfig[AXIS_LX] = PAD_AXIS_LX;
	m_axisPadConfig[AXIS_LY] = PAD_AXIS_LY;
	m_axisPadConfig[AXIS_LZ] = PAD_AXIS_LZ;
	m_axisPadConfig[AXIS_RX] = PAD_AXIS_RX;
	m_axisPadConfig[AXIS_RY] = PAD_AXIS_RY;
	m_axisPadConfig[AXIS_RZ] = PAD_AXIS_RZ;
}

InputInterface::~InputInterface()
{
}

void InputInterface::Update()
{
	// ボタン更新
	for (int i = 0; i < BUTTON_MAX; ++i) {
		// 入力更新
		m_oldButton[i] = m_nowButton[i];
		m_nowButton[i] = IsPressKeyboard(m_buttonKeyConfig[i]);
		m_nowButton[i] |= IsPressGamePad(0,
			static_cast<GamePadButtonKind>(m_buttonPadConfig[i]));

		// リピート更新
		if (m_nowButton[i]) {
			++ m_repeatButtonCount[i];
			if (m_repeatButtonCount[i] > m_repeatWait) {
				m_repeatButtonCount[i] -= m_repeatWait;
				m_repeatButtonCount[i] %= m_repeatTick;
				m_repeatButtonCount[i] += m_repeatWait;
			}
		}
		else {
			m_repeatButtonCount[i] = 0;
		}
	}

	// 軸値更新
	for (int i = 0; i < AXIS_MAX; ++i) {
		m_axis[i] = IsPressKeyboard(m_axisKeyConfig[i][0]) ? -1.0f : 0.0f;
		m_axis[i] += IsPressKeyboard(m_axisKeyConfig[i][1]) ? 1.0f : 0.0f;
		m_axis[i] += GetAxisGamePad(0, 
			static_cast<GamePadAxisKind>(m_axisPadConfig[i]));
		m_axis[i] = min(1.0f, max(-1.0f, m_axis[i]));
	}
}

void InputInterface::Draw()
{
}

void InputInterface::SetRepeat(int wait, int tick)
{
	m_repeatWait = max(wait, 1) + 1;
	m_repeatTick = max(tick, 1);
}

bool InputInterface::IsPress(ButtonKind button) const
{
	return m_nowButton[button];
}
bool InputInterface::IsTrigger(ButtonKind button) const
{
	// (前 ^ 今) & 今
	return (m_oldButton[button] ^ m_nowButton[button]) & m_nowButton[button];
}
bool InputInterface::IsRelease(ButtonKind button) const
{
	// (前 ^ 今) & 前
	return (m_oldButton[button] ^ m_nowButton[button]) & m_oldButton[button];
}
bool InputInterface::IsRepeat(ButtonKind button) const
{
	if (m_repeatButtonCount[button] == 1) {
		return true;
	}
	else if(m_repeatButtonCount[button] == m_repeatWait) {
		return true;
	}
	return false;
}

float InputInterface::GetAxis(AxisKind axis) const
{
	return m_axis[axis];
}

// EOF