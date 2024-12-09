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

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_CHANGE;
	m_nWave = 0;
	m_nCount = 0;
	m_fSinFloat = 0.0f;
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
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	CDevil* pDevil = CDevil::GetListTop();

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//初期の玉座モーション
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);
		
		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_KING, 0);
			pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nWave++;
		}

		break;

	case 1:		//プレイヤーが一定の距離まで歩く

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);

		if (pDevil->GetMotion()->GetFinish())
		{
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);
			m_nWave++;
		}

		break;

	default:

		//pCamera->SetCameraMode(CCamera::CAMERAMODE_FPS);
		//SetEvent(false);
		//m_pPlayer->SetModelDisp(false);
		//m_pBoss->EventJump(60);
		//m_nWave = 0;
		//m_nEventNumber++;
		break;
	}

	m_nCount++;
}

//====================================================================
//開始時演出
//====================================================================
void CEventMovie::StageChangeMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	CDevil* pDevil = CDevil::GetListTop();
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer(0);
	D3DXVECTOR3 PlayerPos = INITVECTOR3;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//初期化

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//カメラをイベント用に変更
		CGame::GetInstance()->GetTime()->SetStopTime(true);	//タイムの進行を止める

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_SIGNAL_DOWN, 0);	//怒りモーションに変更
			m_nWave++;
		}

		break;

	case 1:		//怒りモーション

		if (m_nCount >= 60)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);	//カメラを指定位置に移動
			pCamera->SetCameraPosMode(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f), pDevil->GetPos());	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.01f);	//カメラの目標までのホーミング速度を設定
			pCamera->SetBib(true);	//カメラを振動状態に設定
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);	//モーションをニュートラルに変更

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

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y += 50.0f;
		pPlayer->SetPos(PlayerPos);


		if (m_nCount >= 60)
		{
			//プレイヤーの位置設定
			PlayerPos = pPlayer->GetPos();
			PlayerPos = D3DXVECTOR3(0.0f, 1700.0f, -400.0f);
			pPlayer->SetPos(PlayerPos);

			//カメラの指定位置を設定
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, pPlayer->GetPos().z));
			pCamera->SetHomingSpeed(0.9f);	//カメラの目標までのホーミング速度を設定
			m_fSinFloat = pPlayer->GetPos().y;		//サインカーブ用の初期位置設

			//動き確認用(デバッグ)===========================
			for (int TestX = 0; TestX < 10; TestX++)
			{
				for (int TestY = 0; TestY < 10; TestY++)
				{
					CEffect *pEffect = CEffect::Create();
					pEffect->SetPos(D3DXVECTOR3(PlayerPos.x - 500.0f + TestX * 100.0f, PlayerPos.y - 500.0f + TestY * 100.0f, PlayerPos.z));
					pEffect->SetRadius(50.0f);
					pEffect->SetDel(0.0f);
					pEffect->SetLife(500);
				}
			}
			//===============================================

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	case 6:		//プレイヤーにカメラを向ける

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y = m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 400.0f;
		pPlayer->SetPos(PlayerPos);
		pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_nCount >= 120)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(INITVECTOR3));	//カメラの指定位置を設定
			pCamera->SetHomingSpeed(0.05f);	//カメラの目標までのホーミング速度を設定

			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;


	case 7:		//カメラを下に向ける


		if (m_nCount >= 10)
		{
			m_nCount = 0;		//カウントリセット
			m_nWave++;
		}

		break;

	default:

		CGame::GetInstance()->GetTime()->SetStopTime(false);	//タイムの進行を進める
		pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
		CGame::GetInstance()->SetEvent(false);
		break;
	}

	m_nCount++;
}