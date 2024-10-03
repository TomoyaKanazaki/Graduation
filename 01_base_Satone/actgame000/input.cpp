//==============================================================
//
//���͏���[input.cpp]
//Author:����������
//
//==============================================================
#include "input.h"

//�ÓI�����o�ϐ��錾
LPDIRECTINPUT8 CInput::m_pInput = NULL;		//DirectInput�I�u�W�F�N�g�ւ̃|�C���^

//==============================================================
//�R���X�g���N�^
//==============================================================
CInput::CInput()
{
	//�ϐ�������
	m_pDevice = NULL;		//���̓f�o�C�X�ւ̃|�C���^
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CInput::~CInput()
{

}

//==============================================================
//����������
//==============================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{//�������͂���ĂȂ�������

		//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();		//�A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==============================================================
//�L�[�{�[�h�̃R���X�g���N�^
//==============================================================
CInputKeyboard::CInputKeyboard()
{
	//�ϐ�������
	for (int nCntKey = 0; nCntKey < MAX_INPUT_KEY; nCntKey++)
	{
		m_aKeyStatePress[nCntKey] = NULL;		//�L�[�{�[�h�̃v���X���
		m_aKeyStateTrigger[nCntKey] = NULL;		//�L�[�{�[�h�̃g���K�[���
		m_aKeyStateRelease[nCntKey] = NULL;		//�L�[�{�[�h�̃����[�X���
	}
}

//==============================================================
//�L�[�{�[�h�̃f�X�g���N�^
//==============================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==============================================================
//�L�[�{�[�h�̏���������
//==============================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//����������
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================
//�L�[�{�[�h�̏I������
//==============================================================
void CInputKeyboard::Uninit(void)
{
	//�I������
	CInput::Uninit();
}

//==============================================================
//�L�[�{�[�h�̍X�V����
//==============================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_INPUT_KEY];		//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_INPUT_KEY; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyStatePress[nCntKey];		//�L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//�L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];			//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();						//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//==============================================================
//�L�[�{�[�h�̃v���X�����擾
//==============================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//==============================================================
//�L�[�{�[�h�̃g���K�[�����擾
//==============================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================
//�L�[�{�[�h�̃����[�X�����擾
//==============================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==============================================================
//�p�b�h�̃R���X�g���N�^
//==============================================================
CInputJoyPad::CInputJoyPad()
{
	//�ϐ�������
	for (int nCntKey = 0; nCntKey < NUM_PLAYER; nCntKey++)
	{
		m_aButtonStatePress[nCntKey] = {};		//�{�^���̃v���X���
		m_aButtonStateTrigger[nCntKey] = {};	//�{�^���̃g���K�[���
		m_aButtonStateRelease[nCntKey] = {};	//�{�^���̃g���K�[���
	}
}

//==============================================================
//�p�b�h�̃f�X�g���N�^
//==============================================================
CInputJoyPad::~CInputJoyPad()
{

}

//==============================================================
//�p�b�h�̏���������
//==============================================================
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//����������
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//xinput�̃X�e�[�g�̐ݒ�
	XInputEnable(true);

	for (int nCount = 0; nCount < NUM_PLAYER; nCount++)
	{
		//�������[���N���A����
		memset(&m_aButtonStatePress[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateRelease[nCount], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//==============================================================
//�p�b�h�̏I������
//==============================================================
void CInputJoyPad::Uninit(void)
{
	//xinput�̃X�e�[�g�̐ݒ�
	XInputEnable(false);

	//�I������
	CInput::Uninit();
}

//==============================================================
//�p�b�h�̍X�V����
//==============================================================
void CInputJoyPad::Update(void)
{
	XINPUT_STATE aGamepadState[NUM_PLAYER];
	static int nCntSpece;
	int nCntPad;

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		//���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			m_aButtonStateTrigger[nCntPad].Gamepad.wButtons =
				~m_aButtonStatePress[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;			//�g���K�[

			m_aButtonStateRelease[nCntPad].Gamepad.wButtons =
				((m_aButtonStatePress[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
					& m_aButtonStatePress[nCntPad].Gamepad.wButtons);	//�����[�X

			m_aButtonStatePress[nCntPad] = aGamepadState[nCntPad];		//�v���X
		}
	}
}

//==============================================================
//�p�b�h�̃v���X�����擾
//==============================================================
bool CInputJoyPad::GetPress(int Key, int nPlayer)
{
	return (m_aButtonStatePress[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==============================================================
//�p�b�hRT�̃v���X�����擾
//==============================================================
float CInputJoyPad::GetPressRT(int nPlayer)
{
	float per = (float)m_aButtonStatePress[nPlayer].Gamepad.bRightTrigger / 255.0f;

	return (per);
}

//==============================================================
//�p�b�hLT�̃v���X�����擾
//==============================================================
float CInputJoyPad::GetPressLT(int nPlayer)
{
	float per = (float)m_aButtonStatePress[nPlayer].Gamepad.bLeftTrigger / 255.0f;

	return (per);
}

//==============================================================
//�Q�[���p�b�hRX�̃v���X�����擾
//==============================================================
D3DXVECTOR3 CInputJoyPad::GetPressRX(int nPlayer)
{
	D3DXVECTOR3 Data;
	float fMan = 7000.0f / (float)(SHRT_MAX);
	float fMin = 7000.0f / (float)(SHRT_MIN);

	Data = D3DXVECTOR3((float)m_aButtonStatePress[nPlayer].Gamepad.sThumbRX, (float)m_aButtonStatePress[nPlayer].Gamepad.sThumbRY, 0.0f) / SHRT_MAX;

	if ((fMan >= Data.x && fMin <= Data.x) &&
		(fMan >= Data.y && fMin <= Data.y))
	{
		Data = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return(Data);
}

//==============================================================
//�Q�[���p�b�hLX�̃v���X�����擾
//==============================================================
D3DXVECTOR3 CInputJoyPad::GetPressLX(int nPlayer)
{
	D3DXVECTOR3 Data;
	float fMan = 7000.0f / (float)(SHRT_MAX);
	float fMin = 7000.0f / (float)(SHRT_MIN);

	Data = D3DXVECTOR3((float)m_aButtonStatePress[nPlayer].Gamepad.sThumbLX, (float)m_aButtonStatePress[nPlayer].Gamepad.sThumbLY, 0.0f) / SHRT_MAX;

	if (fMan >= Data.x && fMin <= Data.x &&
		(fMan >= Data.y && fMin <= Data.y))
	{
		Data = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return(Data);
}

//==============================================================
//�Q�[���p�b�h�g���K�[�����擾
//==============================================================
bool CInputJoyPad::GetTrigger(BUTTON Key, int nPlayer)
{
	return (m_aButtonStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==============================================================
//�Q�[���p�b�h�����[�X�����擾
//==============================================================
bool CInputJoyPad::GetRelease(BUTTON Key, int nPlayer)
{
	return (m_aButtonStateRelease[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}