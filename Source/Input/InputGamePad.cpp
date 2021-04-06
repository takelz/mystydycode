#include "InputGamePad.h"
#include <math.h>
#include <Debug/DebugMemory.h>

//----------
// �萔
//----------
const float PAD_THRESHOLD = 0.15f; // �������l
const float PAD_TILT_LENGTH = 1.f - PAD_THRESHOLD; // �X����

//----------
// �\����
//----------
enum POVKind {
	POV_UP,
	POV_RIGHT,
	POV_DOWN,
	POV_LEFT,
	POV_MAX
};
struct GamePadInfo
{
	bool bUse;
	JOYINFOEX data;
	bool pov[POV_MAX];
	float axis[PAD_AXIS_MAX];
};

//----------
// �O���[�o���ϐ�
//----------
GamePadInfo *g_pGamePads;
int g_maxPadNum;
GamePadConnectCallback ConnectCallback;

/**
 * @brief �Q�[���p�b�h������
 * @param[in] maxPad �g�p����p�b�h��
 * @return ����������
 */
HRESULT InitializeGamePad(UINT maxPad)
{
	// �ő�ڑ���
	g_maxPadNum = min(maxPad, joyGetNumDevs());
	if (g_maxPadNum == 0) {
		return E_FAIL;
	}

	// �ڑ���
	g_pGamePads = NEW GamePadInfo[g_maxPadNum];
	for (int i = 0; i < g_maxPadNum; ++i) {
		ZeroMemory(&g_pGamePads[i].data, sizeof(JOYINFOEX));
		g_pGamePads[i].data.dwSize = sizeof(JOYINFOEX);
		g_pGamePads[i].data.dwFlags = JOY_RETURNALL;

		MMRESULT mResult = joyGetPosEx(i, &g_pGamePads[i].data);
		g_pGamePads[i].bUse = mResult == JOYERR_NOERROR;
	}

	ConnectCallback = NULL;

	return S_OK;
}

/**
 * @brief �Q�[���p�b�h�I������
 */
void TerminateGamePad()
{
	SAFE_DELETE_ARRAY(g_pGamePads);
}

/**
 * @brief �Q�[���p�b�h�X�V����
 */
void UpdateGamePad()
{
	for (int i = 0; i < g_maxPadNum; ++i) {
		// �ڑ��m�F
		MMRESULT mResult = joyGetPosEx(i, &g_pGamePads[i].data);
		if (mResult == JOYERR_NOERROR) {
			// POV���͎擾
			BYTE povBit[] = {
				// ��, �E��, �E, �E��, ��, ����, ��, ����
				0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09,
				0x00
			};
			int angle = g_pGamePads[i].data.dwPOV / 4500;
			if (g_pGamePads[i].data.dwPOV == JOY_POVCENTERED) {
				angle = 8;
			}
			for (int pi = 0; pi < POV_MAX; ++pi) {
				g_pGamePads[i].pov[pi] = (povBit[angle] & (1 << pi)) > 0;
			}
			// �����͎擾
			float axisValue[] = {
				static_cast<float>(g_pGamePads[i].data.dwXpos),
				static_cast<float>(g_pGamePads[i].data.dwYpos),
				static_cast<float>(g_pGamePads[i].data.dwZpos),
				static_cast<float>(g_pGamePads[i].data.dwUpos),
				static_cast<float>(g_pGamePads[i].data.dwRpos),
				static_cast<float>(0x7FFF - g_pGamePads[i].data.dwVpos),
			};
			for (int ai = 0; ai < PAD_AXIS_MAX; ++ai) {
				axisValue[ai] = (axisValue[ai] - 0x7FFF) / 0x7FFF;
				if (fabsf(axisValue[ai]) <= PAD_THRESHOLD) {
					axisValue[ai] = 0.0f;
				}
				else {
					float sign = axisValue[ai] / fabsf(axisValue[ai]);
					axisValue[ai] -= sign * PAD_THRESHOLD;
					axisValue[ai] /= PAD_TILT_LENGTH;
				}
				g_pGamePads[i].axis[ai] = axisValue[ai];
			}

			// �Đڑ����ꂽ��֐��Ăяo��
			if (!g_pGamePads[i].bUse) {
				if (ConnectCallback != NULL) {
					ConnectCallback(true);
				}
				g_pGamePads[i].bUse = true;
			}
		}
		else {
			// �ڑ����؂ꂽ��֐��Ăяo��
			if (g_pGamePads[i].bUse) {
				if (ConnectCallback != NULL) {
					ConnectCallback(false);
				}
				g_pGamePads[i].bUse = false;
			}
		}
	}
}

/**
 * @brief �p�b�h�ڑ��X�V���R�[���o�b�N�֐��ݒ�
 * @param[in] func �Ăяo���֐�
 */
void SetGamePadConnectCallback(GamePadConnectCallback func)
{
	ConnectCallback = func;
}

/**
 * @brief ���ݐڑ����̃p�b�h��
 * @return �p�b�h��
 */
int GetGamePadConnectNum()
{
	int count = 0;
	for (int i = 0; i < g_maxPadNum; ++i) {
		if (g_pGamePads[i].bUse) {
			++count;
		}
	}
	return count;
}

/**
 * @brief �Q�[���p�b�h�{�^������
 * @param[in] id �p�b�h�ԍ�
 * @param[in] button �{�^���̎��
 * @return �������
 */
bool IsPressGamePad(UINT id, GamePadButtonKind button)
{
	if (!g_pGamePads[id].bUse) {
		return false;
	}

	bool press = false;
	switch (button) {
	default:
		press = (g_pGamePads[id].data.dwButtons & button) > 0;
		break;
	case PAD_BUTTON_UP:
		press = g_pGamePads[id].pov[POV_UP];
		break;
	case PAD_BUTTON_DOWN:
		press = g_pGamePads[id].pov[POV_DOWN];
		break;
	case PAD_BUTTON_LEFT:
		press = g_pGamePads[id].pov[POV_LEFT];
		break;
	case PAD_BUTTON_RIGHT:
		press = g_pGamePads[id].pov[POV_RIGHT];
		break;
	}
	return press;
}

/**
 * @brief �����̎擾
 * @param[in] id �p�b�h�ԍ�
 * @param[in] axis ���̎��
 * @return ���l
 */
float GetAxisGamePad(UINT id, GamePadAxisKind axis)
{
	if (!g_pGamePads[id].bUse) {
		return 0.0f;
	}
	return g_pGamePads[id].axis[axis];
}