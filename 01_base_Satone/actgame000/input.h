//==============================================================
//
//���͏���[input.h]
//Author:����������
//
//==============================================================
#ifndef _INPUT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "Xinput.h"

#pragma comment(lib, "xinput.lib")

//�}�N����`
#define MAX_INPUT_KEY			(256)		//�L�[�̍ő吔
#define NUM_PLAYER				(4)			//�v���C���[�̐�

//���̓N���X�̒�`
class CInput
{
public:
	CInput();		//�R���X�g���N�^
	virtual ~CInput();		//�f�X�g���N�^

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//����������
	virtual void Uninit(void);			//�I������
	virtual void Update(void) = 0;		//�X�V����

protected:
	static LPDIRECTINPUT8 m_pInput;		//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;		//���̓f�o�C�X�ւ̃|�C���^
};

//�L�[�{�[�h�N���X�̒�`
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();		//�R���X�g���N�^
	~CInputKeyboard();		//�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//����������
	void Uninit(void);			//�I������
	void Update(void);			//�X�V����

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

private:
	BYTE m_aKeyStatePress[MAX_INPUT_KEY];			//�L�[�{�[�h�̃v���X���
	BYTE m_aKeyStateTrigger[MAX_INPUT_KEY];	//�L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[MAX_INPUT_KEY];	//�L�[�{�[�h�̃����[�X���
};

//�p�b�h�N���X�̒�`
class CInputJoyPad : public CInput
{
public:

	//�{�^���z�u�̗�
	enum BUTTON
	{
		BUTTON_UP = 0,		//�\���L�[��
		BUTTON_DOWN,		//�\���L�[��
		BUTTON_LEFT,		//�\���L�[��
		BUTTON_RIGHT,		//�\���L�[�E
		BUTTON_START,		//�X�^�[�g�{�^��
		BUTTON_BACK,		//�o�b�N�{�^��
		BUTTON_LSTICK,		//���X�e�B�b�N��������
		BUTTON_RSTICK,		//�E�X�e�B�b�N��������
		BUTTON_LB,			//LB
		BUTTON_RB,			//RB
		BUTTON_01,			//
		BUTTON_02,			//
		BUTTON_A,			//A�{�^��
		BUTTON_B,			//B�{�^��
		BUTTON_X,			//X�{�^��
		BUTTON_Y,			//Y�{�^��
		BUTTON00,

		BUTTON_MAX
	};

	CInputJoyPad();		//�R���X�g���N�^
	~CInputJoyPad();	//�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//����������
	void Uninit(void);			//�I������
	void Update(void);			//�X�V����

	bool GetPress(int Key, int nPlayer);
	float GetPressRT(int nPlayer);
	float GetPressLT(int nPlayer);
	D3DXVECTOR3 GetPressRX(int nPlayer);
	D3DXVECTOR3 GetPressLX(int nPlayer);

	bool GetTrigger(BUTTON Key, int nPlayer);
	bool GetRelease(BUTTON Key, int nPlayer);

private:
	XINPUT_STATE m_aButtonStatePress[NUM_PLAYER];			//�{�^���̃v���X���
	XINPUT_STATE m_aButtonStateTrigger[NUM_PLAYER];			//�{�^���̃g���K�[���
	XINPUT_STATE m_aButtonStateRelease[NUM_PLAYER];			//�{�^���̃g���K�[���
};

#endif