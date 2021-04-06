#ifndef _MOUSE_H_
#define _MOUSE_H_
#define    DIRECTINPUT_VERSION    0x0800

//�������w�b�_�t�@�C���ǂݍ��݁�����//
#include <dinput.h>    // DirectInput�o�[�W�����̐錾
#include <DirectXMath.h>
#include <array>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//�������񋓌^��`������//
typedef enum
{
    MOUSEBUTTON_LEFT,
    MOUSEBUTTON_RIGHT,
    MOUSEBUTTON_CENTER,
    MOUSEBUTTON_MAX
}MOUSEBUTTON;

//�������N���X�錾������//
class Mouse
{
    private:
        LPDIRECTINPUTDEVICE8 Device; //�f�o�C�X
        POINT Position;              //�J�[�\�����W
        DIMOUSESTATE Current;         //���݂̏��
        DIMOUSESTATE Preview;         //�O��̏��
        DIMOUSESTATE Trigger;         //�g���K���
        DIMOUSESTATE Release;         //�����[�X���
        HWND Handle;                 //�E�B���h�E�n���h��
#ifdef _DEBUG
        DIMOUSESTATE2            MouseState;            // �}�E�X���
        LPDIRECTINPUTDEVICE8    pDIMouse;    // �}�E�X�f�o�C�X

#endif
    public:
        HRESULT Initialize(HWND, LPDIRECTINPUT8);
        void Uninitialize(void);
        void Update(void);

		// ���������Ă��邩
        bool GetHold(MOUSEBUTTON);
		// �J�[�\���̈ʒu
        POINT GetCursor(void);
		// �������Ƃ�
        bool GetRelease(MOUSEBUTTON);
		// �������u��
        bool GetTrigger(MOUSEBUTTON);

        bool  GetWheelTriggerUP(void);
        bool  GetWheelTriggerDOWN(void);
        bool  GetPressWheel(void);
        bool  GetTriggerWheel(void);
        float  GetWheelVal(void);
        DirectX::XMFLOAT2 GetMoveVal(void);
        bool GetPressClick(int Num);
        bool GetTriggerClick(int Num);

};

#endif