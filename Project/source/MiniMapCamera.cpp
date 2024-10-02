//============================================
//
//	ミニマップカメラの処理 [MiniMapCamera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MiniMapCamera.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"

//マクロ定義
#define CAMERA_DISTANCE (100.0f)							//視点と注視点の距離
#define CAMERA_DISTANCE_EVENT (75.0f)						//イベント時の視点と注視点の距離
#define MODEL_DISTANCE (10.0f)								//モデルと注視点の距離
#define CAMERA_SPEED (9.0f)									//カメラの移動スピード
#define CAMERA_VR_SPEED (0.015f)							//カメラの視点スピード
#define CAMERA_PAD_VR_SPEED (0.015f)						//カメラのパッドの視点スピード
#define CAMERA_HOMING (0.2f)								//カメラの追従スピード
#define POS_HOMING (0.9f)									//位置への追従スピード
#define DOWNVIEW_POSV (D3DXVECTOR3(0.0f, 1500.0f, -200.0f))	//見下ろしの視点
#define DOWNVIEW_POSR (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//見下ろしの注視点
#define SIDEVIEW_POSV (D3DXVECTOR3(0.0f, 350.0f, -1000.0f))	//2Dの視点
#define SIDEVIEW_POSR (D3DXVECTOR3(0.0f, 300.0f, 0.0f))		//2Dの注視点

#define ULTIMATE_DISTANCE (300.0f)							//術発動時の視点と注視点距離
#define ULTIMATE_DISTANCEu (300.0f)							//術発動時の高さ
#define ULTIMATE_ROT (0.85f)								//術発動時の向き

//====================================================================
//コンストラクタ
//====================================================================
CMiniMapCamera::CMiniMapCamera()
{
	m_bBib = false;
	m_fBibPowor = 0.0f;
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_posV = INITVECTOR3;
	m_posR = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_PlayerPos = INITVECTOR3;
	m_DelCameraPos = INITVECTOR3;
	ResetCamera();
	CameraMode = CAMERAMODE_DOWNVIEW;
	m_fHomingSpeed = CAMERA_HOMING;

	m_viewport.X = 10;			// 描画する画面の左上X座標
	m_viewport.Y = 510;				// 描画する画面の左上Y座標
	m_viewport.Width = 200;			// 画面の幅
	m_viewport.Height = 200;		// 画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;

	m_FollowTime = 0;
	m_bFollowY = false;
	m_bAttention = false;
	m_fRotMove = INITVECTOR3;
}

//====================================================================
//デストラクタ
//====================================================================
CMiniMapCamera::~CMiniMapCamera()
{

}

//====================================================================
//カメラの初期化処理
//====================================================================
HRESULT CMiniMapCamera::Init(void)
{
	return S_OK;
}

//====================================================================
//カメラの終了処理
//====================================================================
void CMiniMapCamera::Uninit(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================
void CMiniMapCamera::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		ControlCamera();

		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		switch (CameraMode)
		{
		case CAMERAMODE_CONTROL:	//操作カメラの更新
			ControlCamera();
			break;
		case CAMERAMODE_DOWNVIEW:	//見下ろしカメラの更新
			DownviewCamera();
			break;
		}

		break;
	case CScene::MODE_RESULT:

		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;
	}
	////デバッグ表示の取得
	//CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	//pDebugProc->Print("%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	//pDebugProc->Print("%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);
}

//====================================================================
//操作カメラの更新処理
//====================================================================
void CMiniMapCamera::ControlCamera(void)
{
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
	{//入力
		m_rotOld.x = m_rot.x;

		//キーボード
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{
			m_rot.x += CAMERA_VR_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_K) == true)
		{
			m_rot.x -= CAMERA_VR_SPEED;
		}
	}

	if (fabsf(m_rot.x) > fabsf(D3DX_PI * 0.5f))
	{//上限に達した時１フレーム前のrotにもどる
		m_rot.x = m_rotOld.x;
	}

	//キーボード
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= CAMERA_VR_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += CAMERA_VR_SPEED;
	}

	//一周した時の向きの補正
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	if (pInputKeyboard->GetPress(DIK_RSHIFT) == true)
	{
		m_posV.y += CAMERA_SPEED;
		m_posR.y += CAMERA_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_RCONTROL) == true)
	{
		m_posV.y -= CAMERA_SPEED;
		m_posR.y -= CAMERA_SPEED;
	}

	//キーボード
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		m_posR.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		m_posR.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_F) == true)
	{
		m_posR.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		m_posR.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= CAMERA_SPEED * sinf(m_rot.y);
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
}

//====================================================================
//見下ろしカメラの更新処理
//====================================================================
void CMiniMapCamera::DownviewCamera(void)
{
	CPlayer* pPlayer = nullptr;

	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;

	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}

	if (pPlayer == nullptr)
	{
		assert(("プレイヤーカメラの取得に失敗", false));
	}

	D3DXVECTOR3 Pos = pPlayer->GetPos();
	D3DXVECTOR3 Rot = pPlayer->GetRot();

	m_rot.y = 0.0f;
	m_posV.x = Pos.x + sinf(Rot.y) * 10.0f;
	m_posV.y = Pos.y + 1000.0f;
	m_posV.z = Pos.z + cosf(Rot.y) * 10.0f;
	m_posR.x = Pos.x;
	m_posR.y = Pos.y;
	m_posR.z = Pos.z;
}

//====================================================================
//カメラの設定処理
//====================================================================
void CMiniMapCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ビューポートの設定
	m_pDevice->SetViewport(&m_viewport);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		12000.0f);

	//プロジェクションマトリックスの設定
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	m_pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//====================================================================
//カメラの設定処理
//====================================================================
void CMiniMapCamera::ResetCamera(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraDistance = CAMERA_DISTANCE;
	m_EventPosY = 0.0f;

	if (CScene::MODE_GAME == CScene::GetMode())
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}