//============================================
//
//	プレイヤーのレベルエフェクト処理 [LevelModelEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "LevelModelEffect.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "input.h"
#include "sound.h"
#include "Effect.h"
#include "game.h"
#include "time.h"
#include "camera.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float PLAYER_ROT_SPEED = 0.2f;		//プレイヤーの回転スピード
	const float PLAYER_SPEED = 10.0f;		//プレイヤーの速さ
	const float PLAYER_JAMPPOWER = 15.0f;	//プレイヤーのジャンプ力
	const float PLAYER_JAMPWALL = 4.0f;		//ジャンプまでの力の振り幅
	const int JAMP_ACTIONNOT = 4;			//ジャンプから行動出来るまで
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 80.0f, 40.0f);		//横の当たり判定
}

//==========================================
// 静的メンバ変数宣言
//==========================================
CListManager<CLevelModelEffect>* CLevelModelEffect::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CLevelModelEffect::CLevelModelEffect(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_Action = ACTION_WAIT;
	m_Color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	m_fDel = 0.05f;
}

//====================================================================
//デストラクタ
//====================================================================
CLevelModelEffect::~CLevelModelEffect()
{

}

//====================================================================
//生成処理
//====================================================================
CLevelModelEffect* CLevelModelEffect::Create()
{
	CLevelModelEffect* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//プレイヤーの生成
		pPlayer = new CLevelModelEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CLevelModelEffect::Init(void)
{
	SetType(CObject::TYPE_PLAYEREFFECT);

	// キャラクタークラスの生成
	if (m_pCharacter == nullptr)
	{
		m_pCharacter->Create("data\\TXT\\motion_foot_light_spear.txt");
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CLevelModelEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CLevelModelEffect::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// キャラクターの終了処理
	if (m_pCharacter != nullptr)
	{
		// キャラクターの破棄
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CLevelModelEffect::Update(void)
{
	m_Color.a -= m_fDel;

	if (m_pCharacter != nullptr)
	{
		// モデル数を取得
		int nNumModel = m_pCharacter->GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// モデルの取得処理
			CModel* pModel = m_pCharacter->GetModel(nCnt);

			if (pModel != nullptr)
			{
				// 色設定処理
				pModel->SetColorType(CModel::COLORTYPE_TRUE_ALL);
				pModel->SetColor(m_Color);
			}
		}
	}

	if (m_Color.a < 0.0f)
	{
		Uninit();
	}

	////モーションの更新
	//m_pMotion->Update();
}

//====================================================================
//プレイヤーの姿を取得して描画する
//====================================================================
void CLevelModelEffect::SetAllPose(int nType, int nKey, float nCounter)
{
	if (m_pCharacter != nullptr)
	{
		// モーションの取得処理
		CMotion* pMotion = m_pCharacter->GetMotion();

		if (pMotion != nullptr)
		{
			pMotion->SetStopPose(nType, nKey, nCounter);
		}
	}
}

//====================================================================
//プレイヤーの姿を取得して描画する
//====================================================================
void CLevelModelEffect::SetPose(int nType, int nKey, float nCounter, int nModelNumber)
{
	if (m_pCharacter != nullptr)
	{
		// モデル数を取得
		int nNumModel = m_pCharacter->GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// モーションの取得処理
			CModel* pModel = m_pCharacter->GetModel(nCnt);

			if (pModel != nullptr)
			{
				// 色設定処理
				pModel->SetDisp(false);
			}
		}

		// モーションの取得処理
		CMotion* pMotion = m_pCharacter->GetMotion();

		if (pMotion != nullptr)
		{
			pMotion->SetStopPose(nType, nKey, nCounter);
		}
	}

}

//====================================================================
//描画処理
//====================================================================
void CLevelModelEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// キャラクターの描画
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CLevelModelEffect>* CLevelModelEffect::GetList(void)
{
	return m_pList;
}
