//============================================
//
//	演出用のムービー [EventMovie.cpp]
//	Author:sakamoto kai
//
//============================================
#include "EventMovie.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "Devil.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "timer.h"
#include "cross.h"
#include "GamePlayer.h"
#include "bgObjManager.h"

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_START;
	m_nWave = 0;
	m_nCount = 0;
	m_fSinFloat = 0.0f;
	m_pEventModel = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CEventMovie::~CEventMovie()
{

}

//====================================================================
//生成処理
//====================================================================
CEventMovie* CEventMovie::Create()
{
	CEventMovie* pPause = nullptr;

	if (pPause == nullptr)
	{
		//敵の生成
		pPause = new CEventMovie();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEventMovie::Init(void)
{

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEventMovie::Uninit(void)
{
	// 自身を削除する
	delete this;
}

//====================================================================
//更新処理
//====================================================================
void CEventMovie::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// ボタン押されたら演出スキップ
		if (pInputKeyboard->GetTrigger(DIK_RETURN) ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_START, nCnt))
		{
			CGame::GetInstance()->SetEvent(false);	// スキップする
		}
	}

	switch (m_State)
	{
	case CEventMovie::STATE_START:
		//開始時演出
		StartMovie();
		break;

	case CEventMovie::STATE_CHANGE:
		//開始時演出
		StageChangeMovie();
		break;

	case CEventMovie::STATE_END:
		//終了時演出
		EndMovie();
		break;
	default:
		assert(false);
		break;
	}
}

//====================================================================
//描画処理
//====================================================================
void CEventMovie::Draw(void)
{

}

//====================================================================
//開始時演出
//====================================================================
void CEventMovie::StartMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 DevilPos = pDevil->GetPos();
	float CameraDistance = 0.0f;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	if (CGame::GetInstance()->GetEvent() == false)
	{ // イベントスキップされた場合
		CGame::GetInstance()->SetEvent(true);	// スキップしない

		// 最後のフェーズにする
		m_nWave = 5;
		m_nCount = 150;

		pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// デビルの色元に戻す

		// 椅子があったら消す
		if (m_pEventModel != nullptr)
		{
			m_pEventModel->Uninit();
			m_pEventModel = nullptr;
		}
	}

	switch (m_nWave)
	{
	case 0:		//初期化

		//デビルの非表示
		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		//カメラの初期位置を設定
		pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
		pCamera->SetCameraPosMode(D3DXVECTOR3(0.0f, 1000.0f, -3000.0f), DevilPos);
		pCamera->SetHomingSpeed(0.1f);

		m_nCount = 0;	
		m_nWave++;

		break;

	case 1:		//ステージを注目する

		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_nCount >= 20)
		{
			//カメラの初期位置を設定
			pCamera->SetCameraMode(CCamera::CAMERAMODE_AROUND);
			pCamera->SetAroundRot((D3DX_PI * ((float)m_nCount / (float)180) + (D3DX_PI * 1.0f)));
			CameraDistance = sqrtf((3000.0f) * (3000.0f) + (1000.0f) * (1000.0f));
			pCamera->SetAroundDistance(CameraDistance);

			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 2:		//ぐるっとステージを一周する

		//カメラの向きと距離を設定
		pCamera->SetAroundRot((D3DX_PI * ((float)(m_nCount * 1.8f) / (float)240) + (D3DX_PI * 1.0f)));
		CameraDistance = sqrtf((3000.0f) * (3000.0f) + (1000.0f) * (1000.0f));
		pCamera->SetAroundDistance(CameraDistance);

		if (m_nCount >= 240)
		{
			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 3:		//初期の玉座モーション
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.05f);

		if (m_nCount == 5)
		{
			// エフェクトの生成
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		}

		if (m_nCount >= 80)
		{
			pDevil->SetAction(CDevil::ACTION_KING, 0);
			pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nWave++;
		}

		break;

	case 4:		//ボスの出現

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);

		if (m_pEventModel == nullptr)
		{
			m_pEventModel = CObjectX::Create("data\\MODEL\\01_enemy\\03_devil\\chair.x");
			m_pEventModel->SetPos(pDevil->GetPos());
		}

		if (pDevil->GetMotion()->GetFinish())
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(0.0f, 1600.0f, -900.0f),
				D3DXVECTOR3(INITVECTOR3));	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.02f);	//カメラの目標までのホーミング速度を設定

			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);
			m_nWave++;
			m_nCount = 0;
		}

		break;

	case 5:		//

		if (m_pEventModel != nullptr)
		{
			m_pEventModel->SetScaling(D3DXVECTOR3(m_pEventModel->GetScaling().x - 0.015f, m_pEventModel->GetScaling().y - 0.015f, m_pEventModel->GetScaling().z - 0.015f));
			//m_pEventModel->SetPos(D3DXVECTOR3(m_pEventModel->GetPos().x, m_pEventModel->GetPos().y + 3.0f, m_pEventModel->GetPos().z + 5.0f));

			if (m_pEventModel->GetScaling().x <= 0.0f)
			{
				m_pEventModel->Uninit();
				m_pEventModel = nullptr;
			}
		}

		if (m_nCount >= 150)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);

			m_nCount = 0;
			m_nWave++;
		}

		break;

	default:

		CGame::GetInstance()->GetTime()->SetStopTime(false);	//タイムの進行を進める
		CGame::GetInstance()->SetEvent(false);
		m_nWave = 0;
		break;
	}

	m_nCount++;
}

//====================================================================
//第二形態変化演出
//====================================================================
void CEventMovie::StageChangeMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);

	CDevil* pDevil = CDevil::GetListTop();
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer(0);
	D3DXVECTOR3 PlayerPos = INITVECTOR3;

	CPlayer* pPlayer2 = nullptr;
	D3DXVECTOR3 PlayerPos2 = INITVECTOR3;

	if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		pPlayer2 = CGame::GetInstance()->GetPlayer(1);
	}

	if (CGame::GetInstance()->GetEvent() == false)
	{ // イベントスキップされた場合
		m_nWave = 6;
		m_nCount = 120;
		CGame::GetInstance()->SetEvent(true);	// スキップしない
	}

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//初期化
		pCamera->SetBib(false);	//カメラを振動状態に設定
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//カメラをイベント用に変更
		CGame::GetInstance()->GetTime()->SetStopTime(true);	//タイムの進行を止める

		pDevil->SetAction(CDevil::ACTION_NEUTRAL, 60);	//モーションをニュートラルに変更

		pPlayer->PlayerNumberDisp(false);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			pPlayer2->PlayerNumberDisp(false);
		}

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_ANGER, 60);	//怒りモーションに変更
			m_nWave++;
		}

		// 装置系背景モデルの非表示処理
		CBgObjManager::GetInstance()->SetDeviceAppear(false);

		break;

	case 1:		//怒りモーション

		if (m_nCount >= 60)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);	//カメラを指定位置に移動
			pCamera->SetCameraPosMode(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 200.0f, -1000.0f), pDevil->GetPos());	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.01f);	//カメラの目標までのホーミング速度を設定
			pCamera->SetBib(true);	//カメラを振動状態に設定

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	case 2:		//カメラを引いていく


		if (m_nCount >= 200)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y - 100.0f, -1000.0f),
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y - 100.0f, pDevil->GetPos().z));	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.9f);	//カメラの目標までのホーミング速度を設定
			pCamera->SetBib(false);	//カメラを振動しない状態に設定

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	case 3:		//マップを跳ね上がる

		if (m_nCount >= 5)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f),
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, pDevil->GetPos().z));	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.9f);	//カメラの目標までのホーミング速度を設定

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	case 4:		//マップを元に戻す

		pCamera->SetCameraPosMode(
			D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f),
			D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 1000.0f, pDevil->GetPos().z));	//カメラの指定位置を設定
		pCamera->SetHomingSpeed(0.05f);	//カメラの目標までのホーミング速度を設定

		m_nCount = 0;		//カウントリセット
		m_nWave++;

		break;

	case 5:		//カメラが上を向き、オブジェクトが上に飛んでいく

		//プレイヤー上移動
		PlayerPos = pPlayer->GetPos();
		PlayerPos.y += 50.0f;
		pPlayer->SetPos(PlayerPos);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			PlayerPos2 = pPlayer2->GetPos();
			PlayerPos2.y += 50.0f;
			pPlayer2->SetPos(PlayerPos2);
		}

		//クロス上移動
		ShootUpCross();

		if (m_nCount >= 60)
		{
			//プレイヤーの位置設定
			PlayerPos = pPlayer->GetPos();
			PlayerPos = D3DXVECTOR3(0.0f, 1700.0f, -400.0f);
			pPlayer->SetPos(PlayerPos);

			if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
			{
				//プレイヤーの位置設定
				PlayerPos2 = pPlayer2->GetPos();
				PlayerPos2 = D3DXVECTOR3(200.0f, 1900.0f, 100.0f);
				pPlayer2->SetPos(PlayerPos2);
			}

			//十字架の位置設定
			SetPosEventCross();

			//カメラの指定位置を設定
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x + 50.0f, pPlayer->GetPos().y + 200.0f, -550.0f),
				D3DXVECTOR3(pPlayer->GetPos().x + 50.0f, pPlayer->GetPos().y + 200.0f, pPlayer->GetPos().z));
			pCamera->SetHomingSpeed(0.9f);	//カメラの目標までのホーミング速度を設定
			m_fSinFloat = pPlayer->GetPos().y;		//サインカーブ用の初期位置設

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	case 6:		//プレイヤーにカメラを向ける
		CMapSystem::GetInstance()->ResetMap();

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y = m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 200.0f;
		pPlayer->SetPos(PlayerPos);
		pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			PlayerPos2 = pPlayer2->GetPos();
			PlayerPos2.y = (m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 200.0f) + 200.0f;
			pPlayer2->SetPos(PlayerPos2);
			pPlayer2->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		SetPosCross(PlayerPos);

		if (m_nCount >= 120)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(INITVECTOR3));	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.05f);	//カメラの目標までのホーミング速度を設定

			//pPlayer->SetState(CPlayer::STATE_EGG);
			pPlayer->Reivel(PlayerPos);
			pPlayer->SetPos(PlayerPos);

			if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
			{
				pPlayer2->Reivel(PlayerPos2);
				pPlayer2->SetPos(PlayerPos2);
			}

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;


	case 7:		//カメラを下に向ける

		// 装置系背景モデルの非表示処理
		CBgObjManager::GetInstance()->SetDeviceAppear(true);

		if (m_nCount >= 10)
		{
			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	default:

		pPlayer->PlayerNumberDisp(true);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			pPlayer2->PlayerNumberDisp(true);
		}

		CGame::GetInstance()->NextStage();
		CGame::GetInstance()->GetTime()->SetStopTime(false);	//タイムの進行を進める
		pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
		CGame::GetInstance()->SetEvent(false);
		break;
	}

	m_nCount++;
}

//====================================================================
//終了演出
//====================================================================
void CEventMovie::EndMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 DevilPos = pDevil->GetPos();
	float CameraDistance = 0.0f;
	float a;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	if (CGame::GetInstance()->GetEvent() == false)
	{ // イベントスキップされた場合
		m_nWave = 4;
		m_nCount = 120;
		CGame::GetInstance()->SetEvent(true);	// スキップしない
	}

	switch (m_nWave)
	{
	case 0:		//初期化

		//カメラの初期位置を設定
		pCamera->SetBib(true);									//カメラを振動状態に設定
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//カメラをイベント用に変更
		pCamera->SetCameraPosMode(D3DXVECTOR3(0.0f, 1000.0f, -3000.0f), DevilPos);
		pCamera->SetHomingSpeed(0.1f);

		m_nCount = 0;
		m_nWave++;

		break;

	case 1:		//

		if (m_nCount >= 120)
		{
			pCamera->SetBib(false);									//カメラを振動状態に設定
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
			pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);
			pCamera->SetHomingSpeed(0.5f);

			m_fSinFloat = DevilPos.y;

			//デビルのモーション変更
			pDevil->SetAction(CDevil::ACTION_RUN, 0);

			// エフェクトの生成
			D3DXMATRIX mat = {};
			D3DXVECTOR3 ef = useful::CalcMatrix(DevilPos, pDevil->GetRot(), mat);
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));

			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 2:		//

		a = m_fSinFloat + (sinf(D3DX_PI * (float)m_nCount / 120) * 800.0f);

		pDevil->SetRot(D3DXVECTOR3(pDevil->GetRot().x + 0.2f, pDevil->GetRot().y, pDevil->GetRot().z));
		pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x, a, pDevil->GetPos().z));
		pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);

		if (m_nCount >= 120)
		{
			pDevil->SetRot(D3DXVECTOR3(D3DX_PI * 1.0f, 0.0f, 0.0f));
			pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 100.0f, pDevil->GetPos().z));

			//デビルの非表示
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 0);

			m_nCount = 0;
			m_nWave++;
		}

		break;


	case 3:		//

		if (m_nCount >= 120)
		{
			pDevil->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

			//デビルの非表示
			pDevil->SetAction(CDevil::ACTION_RUN, 0);

			m_nCount = 0;
			m_nWave++;

			// エフェクトの生成
			D3DXMATRIX mat = {};
			D3DXVECTOR3 ef = useful::CalcMatrix(DevilPos, pDevil->GetRot(), mat);
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(25.0f, 25.0f, 25.0f));
		}

		break;

	case 4:		//
		
		pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x + 10.0f, pDevil->GetPos().y, pDevil->GetPos().z));
		pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);

		if (m_nCount >= 120)
		{
			//デビルの非表示
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 0);

			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));

			m_nCount = 0;
			m_nWave++;
		}

		break;

	default:

		//CGame::GetInstance()->GetTime()->SetStopTime(false);	//タイムの進行を進める
		//CGame::GetInstance()->SetEvent(false);
		//m_nWave = 0;
		break;
	}

	m_nCount++;
}

//====================================================================
//十字架を打ち上げる処理
//====================================================================
void CEventMovie::ShootUpCross(void)
{
	// 敵のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CCross* pCorss : list)
	{
		D3DXVECTOR3 pos = pCorss->GetPos();
		pos.y += 40.0f;
		pCorss->SetPos(pos);
	}
}

//====================================================================
//十字架の位置設定
//====================================================================
void CEventMovie::SetPosCross(D3DXVECTOR3 Pos)
{
	// 敵のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CCross* pCorss : list)
	{
		D3DXVECTOR3 MyEventpos = pCorss->GetEventPos();
		pCorss->SetPos(D3DXVECTOR3(MyEventpos + Pos));
	}
}

//====================================================================
//十字架の位置設定
//====================================================================
void CEventMovie::SetPosEventCross()
{
	// 敵のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 敵のリストの中身を確認する
	for (CCross* pCorss : list)
	{
		pCorss->SetEventPos(D3DXVECTOR3(
			((float)(rand() % 400)), 
			(float)(rand() % 500), 
			(+400.0f + (float)(rand() % 600))));
	}
}