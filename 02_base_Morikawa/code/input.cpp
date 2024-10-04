//========================================
//
//シューティングアクション[input.cpp]
//Author：森川駿弥
//
//========================================

#include "input.h"

//========================================
//静的メンバ変数
//========================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;
DIMOUSESTATE CInputMouse::m_CurrentMouseState;
DIMOUSESTATE CInputMouse::m_PrevMouseState;

//========================================
//コンストラクタ
//========================================
CInput::CInput() : m_pDevice(nullptr)
{
}

//========================================
//デストラクタ
//========================================
CInput::~CInput()
{
}

//========================================
//初期化
//========================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DirectInputオブジェクトの生成
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
//終了
//========================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{//m_pDeviceがnullptrじゃない時
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{//m_pInputがnullptrじゃない時
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//========================================
//キーボードのコンストラクタ
//========================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_MAX_KEY; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;			//キーボードのプレス情報
		m_aKeyStateTrigger[nCntKey] = 0;	//キーボードのトリガー情報
	}
}

//========================================
//キーボードのデストラクタ
//========================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================
//キーボードの初期化
//========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//キーボードのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//========================================
//キーボードの終了
//========================================
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//========================================
//キーボードの更新
//========================================
void CInputKeyboard::Update()
{
	//キーボードの入力情報
	BYTE aKeyState[NUM_MAX_KEY];

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_MAX_KEY; nCntKey++)
		{
			//キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//キーボードのプレス情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}

	else
	{//キーボードのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//========================================
//キープレス情報の取得
//========================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//========================================
//キートリガー情報の取得
//========================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// コンストラクタ
//========================================
CInputMouse::CInputMouse()
{
}

//========================================
// デストラクタ
//========================================
CInputMouse::~CInputMouse()
{
}

//========================================
// 初期化
//========================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//デバイスのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//========================================
// 終了
//========================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//========================================
// 更新
//========================================
void CInputMouse::Update(void)
{
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
	{
		// 最新のマウス情報を保存する
		m_PrevMouseState = m_CurrentMouseState;

		// マウス座標取得
		GetCursorPos(&m_pPoint);
	}
	else
	{
		//デバイスのアクセス権を獲得
		m_pDevice->Acquire();
	}

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA("BladeKnight", nullptr), &m_pPoint);
}

//========================================
// クリックされた判定
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
// クリックやめた判定
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
//コントローラーのコンストラクタ
//========================================
CInputPad::CInputPad()
{
	memset(&m_aPadState[0], 0, sizeof(m_aPadState));
	memset(&m_aPadStateTrigger[0], 0, sizeof(m_aPadStateTrigger));
	memset(&m_aPadStateRelease[0], 0, sizeof(m_aPadStateRelease));
	memset(&m_aPadStateRepeat[0], 0, sizeof(m_aPadStateRepeat));
}

//========================================
//コントローラーのデストラクタ
//========================================
CInputPad::~CInputPad()
{
}

//========================================
//コントローラーの初期化
//========================================
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	int nCntPad;
	//有効化
	XInputEnable(true);

	for (nCntPad = 0; nCntPad < NUM_MAX_PAD; nCntPad++)
	{//メモリ初期化
		memset(&m_aPadState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aPadStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//========================================
//コントローラーの終了
//========================================
void CInputPad::Uninit(void)
{
	//無効化
	XInputEnable(false);
}

//========================================
//コントローラーの更新
//========================================
void CInputPad::Update(void)
{
	//パッドの入力情報
	XINPUT_STATE aPadState[NUM_MAX_PAD];

	int nCntPad;

	//入力デバイスからデータを取得
	for (nCntPad = 0; nCntPad < NUM_MAX_PAD; nCntPad++)
	{
		if (XInputGetState(nCntPad, &aPadState[nCntPad]) == ERROR_SUCCESS)
		{
			//パッドのトリガー情報を保存
			m_aPadStateTrigger[nCntPad].Gamepad.wButtons =
				~m_aPadState[nCntPad].Gamepad.wButtons
				& aPadState[nCntPad].Gamepad.wButtons;

			//パッドのリリース情報を保存
			m_aPadStateRelease[nCntPad].Gamepad.wButtons =
				m_aPadState[nCntPad].Gamepad.wButtons
				& ~aPadState[nCntPad].Gamepad.wButtons;

			//m_aPadStateTrigger[nCntPad].Gamepad.wButtons =
			//	(m_aPadState[nCntPad].Gamepad.wButtons
			//		^ aPadState[nCntPad].Gamepad.wButtons)
			//	& m_aPadState[nCntPad].Gamepad.wButtons;

			//パッドのプレス情報を保存
			m_aPadState[nCntPad] = aPadState[nCntPad];
		}
	}
}
