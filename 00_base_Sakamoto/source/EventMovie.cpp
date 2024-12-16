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
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	CObjectX* pChair = nullptr;
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 DevilPos = pDevil->GetPos();
	float CameraDistance = 0.0f;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//初期化

		//デビルの非表示
		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		//カメラの初期位置を設定
		pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
		pCamera->SetCameraPosMode(D3DXVECTOR3(0.0f, 1000.0f, -3000.0f), DevilPos);

		m_nCount = 0;	
		m_nWave++;

		break;

	case 1:		//ステージを注目する

		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_nCount >= 60)
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

	//case 0:		//初期の玉座モーション
	//	pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
	//	pCamera->SetHomingSpeed(0.2f);

	//	if (m_nCount >= 180)
	//	{
	//		pDevil->SetAction(CDevil::ACTION_KING, 0);
	//		pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//		m_nWave++;
	//	}

	//	break;

	//case 1:		//ボスの出現

	//	pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
	//	pCamera->SetHomingSpeed(0.2f);

	//	pChair = CObjectX::Create("data\\MODEL\\01_enemy\\03_devil\\chair.x");
	//	pChair->SetPos(pDevil->GetPos());

	//	if (pDevil->GetMotion()->GetFinish())
	//	{
	//		pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);
	//		m_nWave++;
	//		m_nCount = 0;
	//	}

	//	break;

	//case 2:		//ボスの出現

	//	DevilPos = pDevil->GetPos();
	//	DevilPos.z -= 1.0f;
	//	pDevil->SetPos(DevilPos);

	//	if (m_nCount >= 120)
	//	{
	//		m_nCount = 0;
	//		m_nWave++;
	//	}

	//	break;

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

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//初期化
		pCamera->SetBib(false);	//カメラを振動状態に設定
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//カメラをイベント用に変更
		CGame::GetInstance()->GetTime()->SetStopTime(true);	//タイムの進行を止める

		pPlayer->PlayerNumberDisp(false);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			pPlayer2->PlayerNumberDisp(false);
		}

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