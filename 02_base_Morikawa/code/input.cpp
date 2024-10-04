//========================================
//
//�V���[�e�B���O�A�N�V����[input.cpp]
//Author�F�X��x��
//
//========================================

#include "input.h"

//========================================
//�ÓI�����o�ϐ�
//========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;
DIMOUSESTATE CInputMouse::m_CurrentMouseState;
DIMOUSESTATE CInputMouse::m_PrevMouseState;

//========================================
//�R���X�g���N�^
//========================================
CInput::CInput() : m_pDevice(nullptr)
{
}

//========================================
//�f�X�g���N�^
//========================================
CInput::~CInput()
{
}

//========================================
//������
//========================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, 
									  DIRECTINPUT_VERSION, 
									  IID_IDirectInput8,
									  (void **)&m_pInput, 
									  nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//========================================
//�I��
//========================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{//m_pDevice��nullptr����Ȃ���
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr)
	{//m_pInput��nullptr����Ȃ���
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//========================================
//�L�[�{�[�h�̃R���X�g���N�^
//========================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_MAX_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;			//�L�[�{�[�h�̃v���X���
		m_aKeyStateTrigger[nCntKey] = 0;	//�L�[�{�[�h�̃g���K�[���
	}
}

//========================================
//�L�[�{�[�h�̃f�X�g���N�^
//========================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================
//�L�[�{�[�h�̏�����
//========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//========================================
//�L�[�{�[�h�̏I��
//========================================
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//========================================
//�L�[�{�[�h�̍X�V
//========================================
void CInputKeyboard::Update()
{
	//�L�[�{�[�h�̓��͏��
	BYTE aKeyState[NUM_MAX_KEY];

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_MAX_KEY; nCntKey++)
		{
			//�L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//�L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}

	else
	{//�L�[�{�[�h�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//========================================
//�L�[�v���X���̎擾
//========================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================
//�L�[�g���K�[���̎擾
//========================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// �R���X�g���N�^
//========================================
CInputMouse::CInputMouse()
{
}

//========================================
// �f�X�g���N�^
//========================================
CInputMouse::~CInputMouse()
{
}

//========================================
// ������
//========================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//========================================
// �I��
//========================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//========================================
// �X�V
//========================================
void CInputMouse::Update(void)
{
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
	{
		// �ŐV�̃}�E�X����ۑ�����
		m_PrevMouseState = m_CurrentMouseState;

		// �}�E�X���W�擾
		GetCursorPos(&m_pPoint);
	}
	else
	{
		//�f�o�C�X�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA("BladeKnight", nullptr), &m_pPoint);
}

//========================================
// �N���b�N���ꂽ����
//========================================
bool CInputMouse::OnMouseDown(int MouseButton)
{
	if (!(m_PrevMouseState.rgbButtons[MouseButton] & 0x80)
		&& m_CurrentMouseState.rgbButtons[MouseButton] & 0x80)
	{
		return true;
	}

	return false;
}

//========================================
// �N���b�N��߂�����
//========================================
bool CInputMouse::OnMouseUp(int MouseButton)
{
	if (!(m_PrevMouseState.rgbButtons[MouseButton] & 0x80)
		&& !(m_CurrentMouseState.rgbButtons[MouseButton] & 0x80))
	{
		return true;
	}

	return false;
}

//========================================
//�R���g���[���[�̃R���X�g���N�^
//========================================
CInputPad::CInputPad()
{
	memset(&m_aPadState[0], 0, sizeof(m_aPadState));
	memset(&m_aPadStateTrigger[0], 0, sizeof(m_aPadStateTrigger));
	memset(&m_aPadStateRelease[0], 0, sizeof(m_aPadStateRelease));
	memset(&m_aPadStateRepeat[0], 0, sizeof(m_aPadStateRepeat));
}

//========================================
//�R���g���[���[�̃f�X�g���N�^
//========================================
CInputPad::~CInputPad()
{
}

//========================================
//�R���g���[���[�̏�����
//========================================
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	int nCntPad;
	//�L����
	XInputEnable(true);

	for (nCntPad = 0; nCntPad < NUM_MAX_PAD; nCntPad++)
	{//������������
		memset(&m_aPadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aPadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//========================================
//�R���g���[���[�̏I��
//========================================
void CInputPad::Uninit(void)
{
	//������
	XInputEnable(false);
}

//========================================
//�R���g���[���[�̍X�V
//========================================
void CInputPad::Update(void)
{
	//�p�b�h�̓��͏��
	XINPUT_STATE aPadState[NUM_MAX_PAD];

	int nCntPad;

	//���̓f�o�C�X����f�[�^���擾
	for (nCntPad = 0; nCntPad < NUM_MAX_PAD; nCntPad++)
	{
		if (XInputGetState(nCntPad, &aPadState[nCntPad]) == ERROR_SUCCESS)
		{
			//�p�b�h�̃g���K�[����ۑ�
			m_aPadStateTrigger[nCntPad].Gamepad.wButtons =
				~m_aPadState[nCntPad].Gamepad.wButtons
				& aPadState[nCntPad].Gamepad.wButtons;

			//�p�b�h�̃����[�X����ۑ�
			m_aPadStateRelease[nCntPad].Gamepad.wButtons =
				m_aPadState[nCntPad].Gamepad.wButtons
				& ~aPadState[nCntPad].Gamepad.wButtons;

			//m_aPadStateTrigger[nCntPad].Gamepad.wButtons =
			//	(m_aPadState[nCntPad].Gamepad.wButtons
			//		^ aPadState[nCntPad].Gamepad.wButtons)
			//	& m_aPadState[nCntPad].Gamepad.wButtons;

			//�p�b�h�̃v���X����ۑ�
			m_aPadState[nCntPad] = aPadState[nCntPad];
		}
	}
}
