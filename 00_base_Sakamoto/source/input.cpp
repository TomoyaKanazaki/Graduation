//========================================================================================
//
//　入力デバイスの処理[input.cpp]
//	Author:sakamoto kai
//
//========================================================================================

//==========================================
//  定数定義
//==========================================
namespace
{
	const int GAMEPAD_TRIGGER_DEADZONE = 100;	//トリガーのデッドゾーン
	const int TRIGGER_MAX = 255;	//トリガーのマックス
	const float MOUSE_SENS = 0.1f; //マウス感度の補正
}

//静的メンバ変数宣言
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//入力全体---------------------------------------------------------------------------------------
//====================================================================
//コンストラクタ
//====================================================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CInput::~CInput()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_pInput,
			nullptr
		)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//====================================================================
//コンストラクタ
//====================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyStateRerease[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
		m_aKeyState[nCntKey] = 0;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//キーボード---------------------------------------------------------------------------------------
//====================================================================
//初期化処理
//====================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイスの設定
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
	if (FAILED(m_pDevice->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; //キーボードの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRerease[nCntKey] = ~aKeyState[nCntKey] & ((m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey]); //キーボードのリリース情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報の保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのプレス情報の保存
		}
	}
	else
	{
		m_pDevice->Acquire(); //キーボードのアクセス権
	}
}

//====================================================================
//キーボードのプレス情報を取得
//====================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのトリガー情報を取得
//====================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのリリース情報を取得
//====================================================================
bool CInputKeyboard::GetRerease(int nKey)
{
	return (m_aKeyStateRerease[nKey] & 0x80) ? true : false;
}

////ジョイパッド---------------------------------------------------------------------------------------
//====================================================================
//コンストラクタ
//====================================================================
CInputJoypad::CInputJoypad()
{

}

//====================================================================
//デストラクタ
//====================================================================
CInputJoypad::~CInputJoypad()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//Xinputのステートの設定
	XInputEnable(true);
	int nCount;

	for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		//メモリーをクリアする
		memset(&m_JoyKeyState[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_JoyKeyStateTrigger[nCount], 0, sizeof(XINPUT_STATE));
		memset(&m_JoyKeyStateRelease[nCount], 0, sizeof(XINPUT_STATE));

		m_LStickInput = LSTICK_UP;
		m_OutputDOK = false;
		m_OutputUOK = false;
		m_OutputLOK = false;
		m_OutputROK = false;
	}
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CInputJoypad::Uninit(void)
{
	CInput::Uninit();

	//Xinputのステートの設定
	XInputEnable(false);
}

//====================================================================
//更新処理
//====================================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE aGamepadState[MAX_PLAYER];	//ゲームパッドの入力処理
	static int nCountSpece = 0;
	int nCntPad = 0;

	for (nCntPad = 0; nCntPad < MAX_PLAYER; nCntPad++)
	{
		//入力デバイスからデータを所得
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			m_JoyKeyStateTrigger[nCntPad].Gamepad.wButtons =
				~m_JoyKeyState[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;	//トリガー

			m_JoyKeyStateRelease[nCntPad].Gamepad.wButtons =
				((m_JoyKeyState[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
					& m_JoyKeyState[nCntPad].Gamepad.wButtons);	//リリース

			m_JoyKeyStateTrigger[nCntPad].Gamepad.bLeftTrigger =
				~m_JoyKeyState[nCntPad].Gamepad.bLeftTrigger
				& aGamepadState[nCntPad].Gamepad.bLeftTrigger;	//トリガー

			m_JoyKeyStateTrigger[nCntPad].Gamepad.bRightTrigger =
				~m_JoyKeyState[nCntPad].Gamepad.bRightTrigger
				& aGamepadState[nCntPad].Gamepad.bRightTrigger;	//トリガー

			//m_JoyKeyStateTrigger[nCntPad].Gamepad.bRightTrigger = aGamepadState[nCntPad].Gamepad.bRightTrigger;	//プレス

			m_JoyKeyStateTrigger[nCntPad].Gamepad.sThumbLY =
				~m_JoyKeyState[nCntPad].Gamepad.sThumbLY
				& aGamepadState[nCntPad].Gamepad.sThumbLY;	//トリガー

			m_JoyKeyStateTrigger[nCntPad].Gamepad.sThumbLX =
				~m_JoyKeyState[nCntPad].Gamepad.sThumbLX
				& aGamepadState[nCntPad].Gamepad.sThumbLX;	//トリガー

			m_JoyKeyStateRelease[nCntPad].Gamepad.sThumbLY =
				((m_JoyKeyState[nCntPad].Gamepad.sThumbLY
					^ aGamepadState[nCntPad].Gamepad.sThumbLY)
					& m_JoyKeyState[nCntPad].Gamepad.sThumbLY);	//リリース

			m_JoyKeyState[nCntPad] = aGamepadState[nCntPad];	//プレス
		}
	}
}

//====================================================================
//キーボードのプレス情報を取得
//====================================================================
bool CInputJoypad::GetPress(JOYKEY nKey, int nPlayer)
{
	return (m_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================================
//キーボードのトリガー情報を取得
//====================================================================
bool CInputJoypad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return (m_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================================
//キーボードのリリース情報を取得
//====================================================================
bool CInputJoypad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return (m_JoyKeyStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//====================================================================
//ゲームパッドのLトリガーボタンの情報を取得
//====================================================================
bool CInputJoypad::GetTriggerButtonLeft(int nPlayer)
{
	if (m_JoyKeyState[nPlayer].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return m_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
	}

	return false;
}

//====================================================================
//ゲームパッドのRトリガーボタンの情報を取得
//====================================================================
bool CInputJoypad::GetTriggerButtonRight(int nPlayer)
{
	if (m_JoyKeyState[nPlayer].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		return m_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
	}

	return false;
}

//====================================================================
//ゲームパッドのLスティックの情報を取得
//====================================================================
D3DXVECTOR3 CInputJoypad::Get_Stick_Left(int nPlayer)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbLX / (float)SHRT_MAX;
	if (fabsf(Stick.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.x = 0.0f;
	}

	Stick.y = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbLY / (float)SHRT_MAX;
	if (fabsf(Stick.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		Stick.y = 0.0f;
	}

	if (Stick.x > 0.5f || Stick.x < -0.5f &&Stick.y > 0.5f || Stick.y < -0.5f)
	{
		D3DXVec3Normalize(&Stick, &Stick);	//ベクトルを正規化する
	}

	return Stick;
}

//====================================================================
//ゲームパッドのRスティックの情報を取得
//====================================================================
D3DXVECTOR3 CInputJoypad::Get_Stick_Right(int nPlayer)
{
	//ローカル変数宣言
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
	//if (fabsf(Stick.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	//{
	//	Stick.x = 0.0f;
	//}

	Stick.y = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
	//if (fabsf(Stick.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	//{
	//	Stick.y = 0.0f;
	//}

	if (Stick.x > 0.5f || Stick.x < -0.5f && Stick.y > 0.5f || Stick.y < -0.5f)
	{
		D3DXVec3Normalize(&Stick, &Stick);	//ベクトルを正規化する
	}

	return Stick;
}

//====================================================================
//ゲームパッドのLスティックのトリガー情報を取得
//====================================================================
bool CInputJoypad::Get_LStick_Trigger(JOYPAD_LSTICK Type, int nPlayer)
{
	//ローカル変数宣言
	float StickX = 0.0f;
	float StickY = 0.0f;
	bool Output = false;

	StickX = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbLX / (float)SHRT_MAX;
	if (fabsf(StickX) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickX = 0.0f;
	}

	StickY = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbLY / (float)SHRT_MAX;
	if (fabsf(StickY) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickY = 0.0f;
	}

	switch (Type)
	{
	case LSTICK_UP:
		if (StickY >= 0.8f)
		{
			if (m_OutputUOK == false)
			{
				Output = true;
				m_OutputUOK = true;
			}
		}
		else
		{
			m_OutputUOK = false;
		}
		break;

	case LSTICK_DOWN:

		if (StickY <= -0.8f)
		{
			if (m_OutputDOK == false)
			{
				Output = true;
				m_OutputDOK = true;
			}
		}
		else
		{
			m_OutputDOK = false;
		}
		break;
	case LSTICK_RIGHT:

		if (StickX >= 0.8f)
		{
			if (m_OutputROK == false)
			{
				Output = true;
				m_OutputROK = true;
			}
		}
		else
		{
			m_OutputROK = false;
		}
		break;

	case LSTICK_LEFT:
		if (StickX <= -0.8f)
		{
			if (m_OutputLOK == false)
			{
				Output = true;
				m_OutputLOK = true;
			}
		}
		else
		{
			m_OutputLOK = false;
		}
		break;
	}

	return Output;
}

//====================================================================
//ゲームパッドのRスティックのトリガー情報を取得
//====================================================================
bool CInputJoypad::Get_RStick_Trigger(JOYPAD_RSTICK Type, int nPlayer)
{
	//ローカル変数宣言
	float StickX = 0.0f;
	float StickY = 0.0f;
	bool Output = false;

	StickX = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
	if (fabsf(StickX) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickX = 0.0f;
	}

	StickY = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
	if (fabsf(StickY) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickY = 0.0f;
	}

	switch (Type)
	{
	case RSTICK_UP:
		if (StickY >= 0.8f)
		{
			if (m_OutputUOK == false)
			{
				Output = true;
				m_OutputUOK = true;
			}
		}
		else
		{
			m_OutputUOK = false;
		}
		break;

	case RSTICK_DOWN:

		if (StickY <= -0.8f)
		{
			if (m_OutputDOK == false)
			{
				Output = true;
				m_OutputDOK = true;
			}
		}
		else
		{
			m_OutputDOK = false;
		}
		break;
	case RSTICK_RIGHT:

		if (StickX >= 0.8f)
		{
			if (m_OutputROK == false)
			{
				Output = true;
				m_OutputROK = true;
			}
		}
		else
		{
			m_OutputROK = false;
		}
		break;

	case RSTICK_LEFT:
		if (StickX <= -0.8f)
		{
			if (m_OutputLOK == false)
			{
				Output = true;
				m_OutputLOK = true;
			}
		}
		else
		{
			m_OutputLOK = false;
		}
		break;
	}

	return Output;
}

//====================================================================
//ゲームパッドのRスティックのトリガー情報を取得（全方位判定）[Sakai]
//====================================================================
bool CInputJoypad::GetRStickTriggerAllDir(int nPlayer)
{
	//ローカル変数宣言
	float StickX = 0.0f;
	float StickY = 0.0f;
	bool Output = false;

	StickX = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
	if (fabsf(StickX) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickX = 0.0f;
	}

	StickY = (float)m_JoyKeyState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
	if (fabsf(StickY) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)SHRT_MAX)
	{
		StickY = 0.0f;
	}
	
	// 絶対値として求める
	StickX = std::abs(StickX);
	StickY = std::abs(StickY);

	// スティックの判定（片方のみの値 || 斜めの値）
	if (StickX >= 0.8f ||
		StickY >= 0.8f ||
		StickX >= 0.25f && StickY >= 0.25f)
	{
		// トリガー判定
		if (m_OutputOK == false)
		{
			// トリガーオン
			m_OutputOK = true;
			Output = true;
		}
	}
	// スティックの判定（片方のみの値 || 斜めの値）
	else if (
		StickX <= 0.4f ||
		StickY <= 0.4f ||
		StickX <= 0.10f && StickY <= 0.10f)
	{
		// トリガー判定オフ
		m_OutputOK = false;
	}

	return Output;
}

//マウス---------------------------------------------------------------------------------------
//====================================================================
//コンストラクタ
//====================================================================
CInputMouse::CInputMouse()
{

}

//====================================================================
//デストラクタ
//====================================================================
CInputMouse::~CInputMouse()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイスの設定
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, nullptr)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevMouse->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; //相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	//マウスへのアクセス権を取得
	m_pDevMouse->Acquire();

#ifdef _DEBUG
	//マウスカーソルの表示
	ShowCursor(true);
#else
	//マウスカーソルの消去
	ShowCursor(false);
#endif // _DEBUG

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CInputMouse::Uninit(void)
{
	//入力デバイス(マウス)の破棄
	if (m_pDevMouse != nullptr)
	{
		m_pDevMouse->Unacquire();
		m_pDevMouse->Release();
		m_pDevMouse = nullptr;
	}

	CInput::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse; //マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_MouseStateRerease.rgbButtons[nCnt] = ~mouse.rgbButtons[nCnt] & ((m_MouseState.rgbButtons[nCnt] ^ mouse.rgbButtons[nCnt]) & m_MouseState.rgbButtons[nCnt]); //キーボードのリリース情報を保存
			m_MouseStateTrigger.rgbButtons[nCnt] = (m_MouseState.rgbButtons[nCnt] ^ mouse.rgbButtons[nCnt]) & mouse.rgbButtons[nCnt];
		}

		//入力情報の保存
		m_MouseState = mouse;
	}
	else
	{
		m_pDevMouse->Acquire(); //マウスへのアクセス権
	}
}

//====================================================================
//キーボードのプレス情報を取得
//====================================================================
bool CInputMouse::GetPress(MOUSE_PUSH nKey)
{
	return (m_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのトリガー情報を取得
//====================================================================
bool CInputMouse::GetTrigger(MOUSE_PUSH nKey)
{
	return (m_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//====================================================================
//キーボードのリリース情報を取得
//====================================================================
bool CInputMouse::GetRerease(MOUSE_PUSH nKey)
{
	return (m_MouseStateRerease.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  マウスの移動量
//==========================================
D3DXVECTOR3 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR3(((float)m_MouseState.lX) * MOUSE_SENS, ((float)m_MouseState.lY) * MOUSE_SENS, 0.0f);
}

//==========================================
//  マウスの移動量
//==========================================
float CInputMouse::GetWheel(void)
{
	float fInput = (float)m_MouseState.lZ;

	return fInput;
}