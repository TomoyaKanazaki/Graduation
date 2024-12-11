//============================================
//
//	カメラの処理 [camera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "camera.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "model.h"
#include "Scene.h"
#include "sound.h"
#include "Devil.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float CAMERA_DISTANCE = 100.0f;							//視点と注視点の距離
	const float CAMERA_DISTANCE_EVENT = 350.0f;						//イベント時の視点と注視点の距離
	const float CAMERA_HEIGHT_EVENT = 50.0f;						//イベント時の視点の高さ

	const float MODEL_DISTANCE = 10.0f;								//モデルと注視点の距離
	const float CAMERA_SPEED = 9.0f;								//カメラの移動スピード
	const float CAMERA_VR_SPEED = 0.015f;							//カメラの視点スピード
	const float CAMERA_PAD_VR_SPEED = 0.015f;						//カメラのパッドの視点スピード
	const float CAMERA_HOMING = 0.2f;								//カメラの追従スピード
	const float POS_HOMING = 0.9f;									//位置への追従スピード

	const D3DXVECTOR3 DOWNVIEW_POSV = D3DXVECTOR3(0.0f, 1700.0f, -1200.0f);	//見下ろしの視点
	const D3DXVECTOR3 DOWNVIEW_POSR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//見下ろしの注視点

	const D3DXVECTOR3 SIDEVIEW_POSV = D3DXVECTOR3(0.0f, 350.0f, -100.0f);	//2Dの視点
	const D3DXVECTOR3 SIDEVIEW_POSR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);		//2Dの注視点

	const float ULTIMATE_DISTANCE = 150.0f;							//術発動時の視点と注視点距離
	const float ULTIMATE_DISTANCEu = 150.0f;						//術発動時の高さ
	const float ULTIMATE_ROT = 0.5f;								//術発動時の向き

	const D3DXVECTOR3 TITLE_POS_V = D3DXVECTOR3(-1300.0f, 550.0f, 300.0f);	//タイトルカメラの位置(視点)
	const D3DXVECTOR3 TITLE_POS_R = D3DXVECTOR3(-0.0f, 400.0f, 0.0f);		//タイトルカメラの位置(注視点)
	const float TTTLE_DECREASE = 0.05f;								//プレイヤー視点までの距離の減少速度
	const float TTTLE_LENGTH = 0.01f;								//プレイヤー視点までのたどり着いた距離
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CCamera>* CCamera::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera()
{
	m_bBib = false;
	m_fBibPowor = 0.0f;
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_posV = INITVECTOR3;
	m_posR = INITVECTOR3;
	m_SetposV = INITVECTOR3;
	m_SetposR = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_PlayerPos = INITVECTOR3;
	m_DelCameraPos = INITVECTOR3;
	ResetCamera();
	CameraMode = CAMERAMODE_CONTROL;
	m_fHomingSpeed = CAMERA_HOMING;

	m_viewport.X = 0;				// 描画する画面の左上X座標
	m_viewport.Y = 0;				// 描画する画面の左上Y座標
	m_viewport.Width = 1280;		// 画面の幅
	m_viewport.Height = 720;		// 画面の高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	
#ifdef _DEBUG
	CameraMode = CAMERAMODE_CONTROL;
#endif

	m_FollowTime = 0;
	m_bFollowY = false;
	m_bAttention = false;
	m_fRotMove = INITVECTOR3;
	m_nCameraCount = -1;
}

//====================================================================
//デストラクタ
//====================================================================
CCamera::~CCamera()
{

}

//====================================================================
//カメラの初期化処理
//====================================================================
HRESULT CCamera::Init(void)
{
	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CCamera>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//カメラの終了処理
//====================================================================
void CCamera::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}
}

//====================================================================
//カメラの更新処理
//====================================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		if (CameraMode == CAMERAMODE_CONTROL)
		{
			CameraMode = CAMERAMODE_FPS;
		}
		else if (CameraMode == CAMERAMODE_FPS)
		{
			CameraMode = CAMERAMODE_CONTROL;
		}
		//else if (CameraMode == CAMERAMODE_FOLLOW)
		//{
		//	CameraMode = CAMERAMODE_DOWNVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_DOWNVIEW)
		//{
		//	CameraMode = CAMERAMODE_SIDEVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_SIDEVIEW)
		//{
		//	CameraMode = CAMERAMODE_CONTROL;
		//}
	}

#endif // _DEBUG

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		if (CameraMode == CAMERAMODE_SEAMLESS)
		{
			SeamlessModeChangeCamera();
		}
		else
		{
			// タイトルカメラの位置
			TitleCamera();
		}
		
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		switch (CameraMode)
		{
		case CAMERAMODE_CONTROL:	//操作カメラの更新
			ControlCamera();
			break;
		case CAMERAMODE_FOLLOW:		//追従カメラの更新
			FollowCamera();
			break;
		case CAMERAMODE_FIXEDNOW:		//固定カメラの更新
			FixedNow();
			break;
		case CAMERAMODE_BETWEEN:		//２点間カメラの更新
			BetWeen();
			break;
		case CAMERAMODE_DOWNVIEW:	//見下ろしカメラの更新
			DownviewCamera();
			break;
		case CAMERAMODE_SIDEVIEW:	//2Dカメラの更新
			SideviewCamera();
			break;
		case CAMERAMODE_EVENTBOSS:	//イベント中のボス注目カメラの更新
			EventBossCamera();
			break;
		case CAMERAMODE_FPS:		//一人称
			FPSCamera();
			break;
		case CAMERAMODE_FPSCOMPLEMENT:		//一人称変更時の補完用カメラ
			FPSComplementCamera();
			break;
		case CAMERAMODE_SETPOS:	//術発動中のプレイヤー見下ろし視点
			SetPosCamera();
			break;
		case CAMERAMODE_SEAMLESS:
			SeamlessModeChangeCamera();
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
	//デバッグ表示の取得
	DebugProc::Print(DebugProc::POINT_LEFT, "[視点]%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[注視点]%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);
}

//====================================================================
//操作カメラの更新処理
//====================================================================
void CCamera::ControlCamera(void)
{
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
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


	//キーボード
	if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	{
		m_posV.y += CAMERA_SPEED;
		m_posR.y += CAMERA_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{
		m_posV.y -= CAMERA_SPEED;
		m_posR.y -= CAMERA_SPEED;
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

	//マウス
	if (pInputMouse->GetPress(CInputMouse::PUSH_LEFT) == true)
	{
		m_rot.y += pInputMouse->GetMouseMove().x * 0.1f;
		m_rot.x -= pInputMouse->GetMouseMove().y * 0.1f;
	}
	if (pInputMouse->GetPress(CInputMouse::PUSH_RIGHT) == true)
	{
		m_posR.z -= pInputMouse->GetMouseMove().y * (CAMERA_SPEED * cosf(m_rot.y));
		m_posR.x += pInputMouse->GetMouseMove().x * (CAMERA_SPEED * sinf(m_rot.y));

		m_posV.z -= pInputMouse->GetMouseMove().y * (CAMERA_SPEED * cosf(m_rot.y));
		m_posV.x += pInputMouse->GetMouseMove().x * (CAMERA_SPEED * sinf(m_rot.y));
	}
	if (pInputMouse->GetWheel() > 0.0f)
	{
		m_posV.y += CAMERA_SPEED * 3.0f;
		m_posR.y += CAMERA_SPEED * 3.0f;
	}
	if (pInputMouse->GetWheel() < 0.0f)
	{
		m_posV.y -= CAMERA_SPEED * 3.0f;
		m_posR.y -= CAMERA_SPEED * 3.0f;
	}

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

	if (CGame::GetInstance()->GetMode() == CScene::MODE_GAME ||
		CGame::GetInstance()->GetMode() == CScene::MODE_TUTORIAL)
	{
		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
}

//====================================================================
//追従カメラの更新処理
//====================================================================
void CCamera::FollowCamera(void)
{
	//デバイスの取得
	//CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	//CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	//CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//プレイヤーの取得
	//CPlayer* pPlayer = CGame::GetPlayer(0);
	//ボスの取得
	//CBoss* pBoss = CGame::GetBoss();

	//m_fRotMove = m_fRotMove * 0.5f;

	//if (pPlayer->GetJump() == false)
	//{
	//	if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
	//	{//入力
	//		m_rotOld.x = m_rot.x;

	//		//キーボード
	//		if (pInputKeyboard->GetPress(DIK_I) == true)
	//		{
	//			m_fRotMove.x += CAMERA_VR_SPEED;
	//		}
	//		if (pInputKeyboard->GetPress(DIK_K) == true)
	//		{
	//			m_fRotMove.x -= CAMERA_VR_SPEED;
	//		}

	//		//右スティックの上下視点移動入力
	//		m_fRotMove.x += pInputJoypad->Get_Stick_Right(0).y * CAMERA_PAD_VR_SPEED;

	//		m_fRotMove.x -= pInputMouse->GetMouseMove().y * CAMERA_VR_SPEED;

	//		m_rot.x += m_fRotMove.x;
	//	}
	//}

	//if (m_rot.x > D3DX_PI * 0.05f || m_rot.x < -(D3DX_PI * 0.45f))
	//{//上限に達した時１フレーム前のrotにもどる
	//	m_rot.x = m_rotOld.x;
	//}

	////キーボード
	//if (pInputKeyboard->GetPress(DIK_J) == true)
	//{
	//	m_fRotMove.y -= CAMERA_VR_SPEED;

	//}
	//if (pInputKeyboard->GetPress(DIK_L) == true)
	//{
	//	m_fRotMove.y += CAMERA_VR_SPEED;
	//}

	////右スティックの左右視点移動入力
	//m_fRotMove.y += pInputJoypad->Get_Stick_Right(0).x * CAMERA_PAD_VR_SPEED;

	//m_fRotMove.y += pInputMouse->GetMouseMove().x * CAMERA_VR_SPEED;

	//m_rot.y += m_fRotMove.y;

	//一周した時の向きの補正
	//if (m_rot.y > D3DX_PI * 1.0f)
	//{
	//	m_rot.y -= D3DX_PI * 2.0f;
	//}
	//else if (m_rot.y < -D3DX_PI * 1.0f)
	//{
	//	m_rot.y += D3DX_PI * 2.0f;
	//}

	//m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	//m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	//m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

	//m_posVDest.x = pPlayer->GetCameraPos().x + sinf(-pPlayer->GetRot().y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	//m_posVDest.z = pPlayer->GetCameraPos().z + cosf(-pPlayer->GetRot().y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	//m_posVDest.y = pPlayer->GetCameraPos().y + 50.0f + sinf(-m_rot.x) * CAMERA_DISTANCE;

	//if (m_bBib == true)
	//{
	//	m_fBibPowor += 0.4f;

	//	m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;
	//	m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
	//	m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);

	//	if (pPlayer->GetJump() == false)
	//	{
	//		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
	//		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
	//	}
	//}
	//else
	//{
	//	m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed * 0.05f;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed * 0.05f;
	//	m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 1.0f;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 1.0f;

	//	if (pPlayer->GetJump() == false)
	//	{
	//		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
	//		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
	//	}
	//}


	//注目の切り替え
	//if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true ||
	//	pInputJoypad->GetTrigger(CInputJoypad::BUTTON_L, 0) == true)
	//{
	//	ゲームのSEを再生する
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ATTENTION);

	//	m_bAttention = m_bAttention ? false : true;
	//}

	//if (m_bAttention == true)
	//{
	//	m_FollowTime = 180;
	//}

	////モデルが止まった時に正面を向く処理
	//float fRotMove, fRotDest, fRotDiff;

	//if (m_FollowTime > 0)
	//{
	//	m_FollowTime--;

	//	fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));	//現在の向き
	//	fRotDest = atan2f(pBoss->GetPos().x - pPlayer->GetPos().x, pBoss->GetPos().z - pPlayer->GetPos().z);	//目的の向き
	//	//fRotDest = atan2f(sinf(pPlayer->GetRot().y + D3DX_PI), cosf(pPlayer->GetRot().y + D3DX_PI));	//目的の向き
	//	fRotDiff = fRotDest - fRotMove;									 //差分

	//	if (fRotDiff > D3DX_PI * 1.0f)
	//	{
	//		fRotDiff -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotDiff < -D3DX_PI * 1.0f)
	//	{
	//		fRotDiff += D3DX_PI * 2.0f;
	//	}

	//	fRotMove += fRotDiff * 0.075f;

	//	if (fRotMove > D3DX_PI * 1.0f)
	//	{
	//		fRotMove -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotMove < -D3DX_PI * 1.0f)
	//	{
	//		fRotMove += D3DX_PI * 2.0f;
	//	}
	//	m_rot.y = fRotMove;
	//}
}

//====================================================================
//現在の位置で固定
//====================================================================
void CCamera::FixedNow(void)
{

}

//====================================================================
//２点間カメラ(Boss・Player)
//====================================================================
void CCamera::BetWeen(void)
{
	//プレイヤーの取得
	D3DXVECTOR3 PlayerPos = CGame::GetInstance()->GetPlayer(0)->GetPos();
	//ボスの取得
	D3DXVECTOR3 BossPos = INITVECTOR3;

	D3DXVECTOR3 BetWeenPos = PlayerPos + ((BossPos - PlayerPos) * 0.5f);

	//一周した時の向きの補正
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * m_CameraDistance;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * m_CameraDistance;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * m_CameraDistance;

	m_posVDest.x = BetWeenPos.x + sinf(m_rot.y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	m_posVDest.z = BetWeenPos.z + cosf(m_rot.y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	m_posVDest.y = BetWeenPos.y + 50.0f + sinf(-m_rot.x) * m_CameraDistance;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.4f;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed * 1.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed * 1.0f;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 1.0f;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 1.0f;
	}

	if (m_bAttention == true)
	{
		m_FollowTime = 180;
	}
}

//====================================================================
//見下ろしカメラの更新処理
//====================================================================
void CCamera::DownviewCamera(void)
{
	m_rot.y = 0.0f;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.4f;

		m_posV.x += (DOWNVIEW_POSV.x - m_posV.x) * m_fHomingSpeed + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posV.y += (DOWNVIEW_POSV.y - m_posV.y) * m_fHomingSpeed;
		m_posV.z += (DOWNVIEW_POSV.z - m_posV.z) * m_fHomingSpeed + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posR.x += (DOWNVIEW_POSR.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (DOWNVIEW_POSR.y - m_posR.y) * m_fHomingSpeed;
		m_posR.z += (DOWNVIEW_POSR.z - m_posR.z) * m_fHomingSpeed;
	}
	else
	{
		m_posV.x += (DOWNVIEW_POSV.x - m_posV.x) * m_fHomingSpeed;
		m_posV.y += (DOWNVIEW_POSV.y - m_posV.y) * m_fHomingSpeed;
		m_posV.z += (DOWNVIEW_POSV.z - m_posV.z) * m_fHomingSpeed;
		m_posR.x += (DOWNVIEW_POSR.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (DOWNVIEW_POSR.y - m_posR.y) * m_fHomingSpeed;
		m_posR.z += (DOWNVIEW_POSR.z - m_posR.z) * m_fHomingSpeed;
	}
}

//====================================================================
//2Dカメラの更新処理
//====================================================================
void CCamera::SideviewCamera(void)
{
	m_rot.y = 0.0f;
	m_posV.x += (SIDEVIEW_POSV.x - m_posV.x) * m_fHomingSpeed;
	m_posV.y += (SIDEVIEW_POSV.y - m_posV.y) * m_fHomingSpeed;
	m_posV.z += (SIDEVIEW_POSV.z - m_posV.z) * m_fHomingSpeed;
	m_posR.x += (SIDEVIEW_POSR.x - m_posR.x) * m_fHomingSpeed;
	m_posR.y += (SIDEVIEW_POSR.y - m_posR.y) * m_fHomingSpeed;
	m_posR.z += (SIDEVIEW_POSR.z - m_posR.z) * m_fHomingSpeed;
}

//====================================================================
//イベント中のボス注目カメラの更新処理
//====================================================================
void CCamera::EventBossCamera(void)
{
	//ボスの取得
	CDevil* pDevil = CDevil::GetListTop();

	m_posVDest.x = pDevil->GetPos().x + sinf(m_rot.y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.z = pDevil->GetPos().z + cosf(m_rot.y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.y = pDevil->GetPos().y + CAMERA_HEIGHT_EVENT + sinf(-m_rot.x) * CAMERA_DISTANCE_EVENT + 50.0f;

	m_posRDest.x = m_posVDest.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.z = m_posVDest.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.y = m_posVDest.y + sinf(m_rot.x) * CAMERA_DISTANCE_EVENT + 10.0f;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.8f;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed * 5.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed;
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed;
	}
}

//====================================================================
//一人称カメラの更新処理
//====================================================================
void CCamera::FPSCamera(void)
{
	CPlayer* pPlayer = nullptr;

	pPlayer = CGame::GetInstance()->GetPlayer(0);

	m_posV.x = pPlayer->GetPos().x;
	m_posV.y = pPlayer->GetPos().y + 50.0f;
	m_posV.z = pPlayer->GetPos().z;
	m_rot.x = 0.0f;
	m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//注視点の情報を出力する
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;
}

//====================================================================
//一人称変更時の補完用カメラの更新処理
//====================================================================
void CCamera::FPSComplementCamera(void)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = CGame::GetInstance()->GetPlayer(0);


	m_posVDest.x = pPlayer->GetPos().x;
	m_posVDest.y = pPlayer->GetPos().y + 50.0f;
	m_posVDest.z = pPlayer->GetPos().z;
	m_rot.x = 0.0f;
	m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//注視点の情報を出力する
	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

	m_posR.x += (m_posRDest.x - m_posR.x) * 0.6f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.6f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.6f;

	m_posV.x += (m_posVDest.x - m_posV.x) * 0.6f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.6f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.6f;

	if (m_nCameraCount > 0)
	{
		m_nCameraCount--;
	}
	else
	{
		CameraMode = CAMERAMODE_FPS;
	}
}

//====================================================================
//指定した位置に動くカメラ
//====================================================================
void CCamera::SetPosCamera(void)
{
	m_posVDest.x = m_SetposV.x;
	m_posVDest.z = m_SetposV.z;
	m_posVDest.y = m_SetposV.y;

	m_posRDest.x = m_SetposR.x;
	m_posRDest.z = m_SetposR.z;
	m_posRDest.y = m_SetposR.y;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.8f;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed * 5.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed;
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed;
	}
}

//====================================================================
//タイトルカメラ
//====================================================================
void CCamera::TitleCamera(void)
{
	// 視点
	m_posV = TITLE_POS_V;
	// 注視点
	m_posR = TITLE_POS_R;
}

//====================================================================
//モード切り替え時のシームレス(繋ぎ目のない)カメラ
//====================================================================
void CCamera::SeamlessModeChangeCamera(void)
{
	//CPlayer* pPlayer = nullptr;

	////プレイヤーの取得
	//switch (CScene::GetMode())
	//{
	//case CScene::MODE_TITLE:

	//	pPlayer = CTitle::GetPlayer();

	//	break;

	//case CScene::MODE_GAME:

	//	pPlayer = CGame::GetPlayer();

	//	break;

	//case CScene::MODE_TUTORIAL:

	//	pPlayer = CTutorial::GetPlayer();

	//	break;
	//}

	//if (!pPlayer)
	//{
	//	return;
	//}

	//D3DXVECTOR3 posDiffv = {};	// 視点の差分
	//D3DXVECTOR3 posDiffr = {};	// 注視点の差分
	//D3DXVECTOR3 rotDiff = {};	// 向きの差分

	//D3DXVECTOR3 posPlayer = {	// プレイヤー位置
	//	pPlayer->GetPos().x, pPlayer->GetPos().y + 50.0f, pPlayer->GetPos().z
	//};

	//// 向きの設定
	//m_rot.x = pPlayer->GetCameraHeightRot();
	//m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//D3DXVECTOR3 posPlayerR = {	// プレイヤー注視点
	//	posPlayer.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE,
	//	posPlayer.y + sinf(m_rot.x) * CAMERA_DISTANCE,
	//	posPlayer.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE,
	//};

	//// プレイヤー(目標)と視点の差を求める
	//posDiffv.x = posPlayer.x - m_posV.x;
	//posDiffv.y = posPlayer.y - m_posV.y;
	//posDiffv.z = posPlayer.z - m_posV.z;

	//// プレイヤー(目標)と注視点の差を求める
	//posDiffr.x = posPlayerR.x - m_posR.x;
	//posDiffr.y = posPlayerR.y - m_posR.y;
	//posDiffr.z = posPlayerR.z - m_posR.z;

	//// 視点の更新
	//m_posV.x += posDiffv.x * TTTLE_DECREASE;
	//m_posV.y += posDiffv.y * TTTLE_DECREASE;
	//m_posV.z += posDiffv.z * TTTLE_DECREASE;

	//// 視点の更新
	//m_posR.x += posDiffr.x * TTTLE_DECREASE;
	//m_posR.y += posDiffr.y * TTTLE_DECREASE;
	//m_posR.z += posDiffr.z * TTTLE_DECREASE;

	//// 長さを計算
	//float fLength = sqrtf((posPlayer.x - m_posV.x) * (posPlayer.x - m_posV.x) + (posPlayer.y - m_posV.y) * (posPlayer.y - m_posV.y));

	//if (fLength <= TTTLE_LENGTH)
	//{// 数値以下ならモードをチュートリアルへ
	//	CScene::SetMode(CScene::MODE_TUTORIAL);
	//}
}

//====================================================================
//カメラの設定処理
//====================================================================
void CCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ビューポートの設定
	m_pDevice->SetViewport(&m_viewport);

	CManager::GetInstance()->SetGameMtxView(m_mtxView);
	CManager::GetInstance()->SetGamemtxProjection(m_mtxProjection);
	CManager::GetInstance()->SetGameViewport(m_viewport);

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
void CCamera::ResetCamera(void)
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

//====================================================================
//リスト取得
//====================================================================
CListManager<CCamera>* CCamera::GetList(void)
{
	return m_pList;
}
