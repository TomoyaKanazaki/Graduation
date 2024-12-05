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

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_START;
	m_nWave = 0;
	m_nCount = 0;
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

	D3DXVECTOR3 DevilPos;

	if (CGame::GetInstance()->GetDevil())
	{
		DevilPos = CGame::GetInstance()->GetDevil()->GetPos();
	}

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	float EventFinish = 0.0f;

	switch (m_nWave)
	{
	case 0:		//横から見てる視点にする
		pCamera->SetCameraMode(CCamera::CAMERAMODE_SIDEVIEW);
		pCamera->SetHomingSpeed(0.2f);

		m_nWave++;
		break;

	case 1:		//プレイヤーが一定の距離まで歩く

		//if (m_pPlayer->GetPos().x <= 450.0f)
		//{//一定の距離に到達した場合に3人称視点に変更する

		//	pCamera->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		//	pCamera->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		//	pCamera->SetHomingSpeed(0.02f);
		//	m_nWave++;

		//}
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
}