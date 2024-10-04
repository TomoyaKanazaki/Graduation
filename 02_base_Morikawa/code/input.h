//========================================
//
//�V���[�e�B���O�A�N�V����[input.h]
//Author�F�X��x��
//
//========================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//========================================
//���C�u�����̃����N
//========================================
#pragma comment (lib,"dinput8.lib")  //���͏����ɕK�v
#pragma comment (lib,"xinput.lib")
#pragma comment (lib, "dxguid.lib")

//**********************************************************
//�萔��`
//**********************************************************
namespace
{
const int NUM_MAX_KEY = 256;	// �L�[�̍ő吔
const int NUM_MAX_PAD = 4;		// �R���g���[���[�̍ő吔
const int NUM_MAX_BUTTON = 4;	// �}�E�X�{�^���̍ő吔
}

//========================================
//�C���v�b�g�N���X
//========================================
class CInput
{
public:
	CInput();			//�R���X�g���N�^
	virtual ~CInput();	//�f�X�g���N�^

	//�����o�֐�
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;		//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;		//���̓f�o�C�X�ւ̃|�C���^
};

//========================================
//�L�[�{�[�h�N���X
//========================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();	//�R���X�g���N�^
	~CInputKeyboard();	//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

private:
	//�����o�ϐ�
	BYTE m_aKeyState[NUM_MAX_KEY];			//�L�[�{�[�h�̃v���X���
	BYTE m_aKeyStateTrigger[NUM_MAX_KEY];	//�L�[�{�[�h�̃g���K�[���
};

//========================================
// �}�E�X�N���X
//========================================
class CInputMouse : public CInput
{
public:
	enum MouseButton
	{// �}�E�X�{�^��
		BUTTON_L = 0,
		BUTTON_R,
		BUTTON_WHEEL,
		BUTTON_MAX
	};

	CInputMouse();	// �R���X�g���N�^
	~CInputMouse();	// �f�X�g���N�^

	//�����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool OnMouseDown(int MouseButton);		// �N���b�N���ꂽ����
	bool OnMouseUp(int MouseButton);		// �N���b�N��߂�����
	D3DXVECTOR3 GetMouseVelocity();			// �ړ��ʎ擾

private:
	static DIMOUSESTATE m_CurrentMouseState;	// �}�E�X�̌��݂̓��͏��
	static DIMOUSESTATE m_PrevMouseState;		// �}�E�X�̈�t���[���O�̓��͏��
	POINT m_pPoint;		// �}�E�X���W
};

//========================================
//�R���g���[���[�N���X
//========================================
class CInputPad : public CInput
{
public:
	enum JOYKEY
	{
		BUTTON_UP,		//�\����
		BUTTON_DOWN,	//�\����
		BUTTON_LEFT,	//�\����
		BUTTON_RIGHT,	//�\����
		BUTTON_START,	//START
		BUTTON_BACK,	//BACK
		BUTTON_PUSHING_L,     //���X�e�B�b�N��������
		BUTTON_PUSHING_R,     //�E�X�e�B�b�N��������
		BUTTON_LB,		//LB
		BUTTON_RB,		//RB
		BUTTON_10,		//�s��(�g���K�[)
		BUTTON_11,		//�s��(�g���K�[)
		BUTTON_A,		//A�{�^��
		BUTTON_B,		//B�{�^��
		BUTTON_X,		//X�{�^��
		BUTTON_Y,		//Y�{�^��
		BUTTON_L_STICK,	//���X�e�B�b�N
		BUTTON_17,		//�s��
		BUTTON_18,		//�s��
		BUTTON_R_STICK,	//�E�X�e�B�b�N
		BUTTONSTATE_MAX
	};

	CInputPad();	// �R���X�g���N�^
	~CInputPad();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey, int nPlayer) {return (m_aPadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;}				// �v���X���擾
	bool GetTrigger(int nKey, int nPlayer) { return (m_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false; }	// �g���K�[���擾
	bool GetRelease(int nKey, int nPlayer) { return (m_aPadStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false; }	// �����[�X���擾
	
	bool GetRepeat(int nKey, int nPlayer) { return (m_aPadStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false; }		// ���s�[�g���擾
	SHORT GetLStickXPress(JOYKEY Key, int nPlayer) { return m_aPadState[nPlayer].Gamepad.sThumbLX; }	//L�X�e�B�b�N��X���v���X���擾
	SHORT GetLStickYPress(JOYKEY Key, int nPlayer) { return m_aPadState[nPlayer].Gamepad.sThumbLY; }	//L�X�e�B�b�N��Y���v���X���擾
	
	SHORT GetRStickXPress(JOYKEY Key, int nPlayer) { return m_aPadState[nPlayer].Gamepad.sThumbRX; }	//R�X�e�B�b�N��X���v���X���擾
	SHORT GetRStickYPress(JOYKEY Key, int nPlayer) { return m_aPadState[nPlayer].Gamepad.sThumbRY; }	//R�X�e�B�b�N��Y���v���X���擾

private:
	// �����o�ϐ�
	XINPUT_STATE m_aPadState[NUM_MAX_PAD];			// �p�b�h�̏��
	XINPUT_STATE m_aPadStateTrigger[NUM_MAX_PAD];	// �p�b�h�̃g���K�[���
	XINPUT_STATE m_aPadStateRelease[NUM_MAX_PAD];	// �p�b�h�̃����[�X���
	XINPUT_STATE m_aPadStateRepeat[NUM_MAX_PAD];	// �p�b�h�̃����[�X���
};

#endif