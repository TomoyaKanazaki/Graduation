//============================================
//
//	ゲームのプレイヤー [TutorialPlayer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "GamePlayer.h"
#include "object.h"
#include "manager.h"
#include "MyEffekseer.h"
#include "Item.h"
#include "sound.h"
#include "game.h"
#include "LifeUi.h"
#include "number.h"

//===========================================
// 定数定義
//===========================================
namespace
{

}

//===========================================
// 静的メンバ変数宣言
//===========================================

//====================================================================
//コンストラクタ
//====================================================================
CGamePlayer::CGamePlayer(int nPriority) : CPlayer(nPriority)
{
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CGamePlayer::~CGamePlayer()
{

}

//====================================================================
//生成処理
//====================================================================
CGamePlayer* CGamePlayer::Create(int PlayNumber)
{
	CGamePlayer* pPlayer = new CGamePlayer();

	// メモリの確保に失敗した場合nullを返す
	if (pPlayer == nullptr) { assert(false); return nullptr; }

	// 初期化処理に失敗した場合nullを返す
	if (FAILED(pPlayer->Init(PlayNumber)))
	{
		assert(false);
		delete pPlayer;
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CGamePlayer::Init(int PlayNumber)
{
	CPlayer::Init(PlayNumber);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGamePlayer::Uninit(void)
{
	// キャラクタークラスの終了（継承）
	CPlayer::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CGamePlayer::Update(void)
{
	CPlayer::Update();
}

//====================================================================
//描画処理
//====================================================================
void CGamePlayer::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CPlayer::Draw();
}

//====================================================================
//ダメージ処理
//====================================================================
void CGamePlayer::Death(void)
{
	if (GetState() != STATE_EGG && GetState() != STATE_DEATH)
	{
		// 昇天エフェクト
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(GetPos(), GetRot(), mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ACENTION, false, ef, GetRot(), D3DXVECTOR3(100.0f, 100.0f, 100.0f));

		SetLife(GetLife() - 1);

		// 聖書を所持しているときにその場に聖書を落とす
		if (GetItemType() == TYPE_BIBLE)
		{
			// 聖書生成
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GetGrid()));
		}

		if (GetLife() < 0)
		{
			// 死亡音
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);

			CGame::GetInstance()->SetGameEnd(true);
			CGame::GetInstance()->SetGameClear(false);
			CManager::GetInstance()->SetStage(0);
		}
		else
		{
			if (GetLifeUI() != nullptr)
			{
				GetLifeUI()->GetNumber()->SetNumber(GetLife());
			}

			SetState(STATE_DEATH);
			SetMove(INITVECTOR3);
			SetObjMoveX(0.0f);
			SetObjMoveZ(0.0f);
			SetStateCount(150);

			// ダメージ音(仮)
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);
		}

		// アイテムを所持していない状態にする
		SetItemType(TYPE_NONE);
	}
}