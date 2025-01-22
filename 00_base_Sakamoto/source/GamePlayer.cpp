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
#include "score.h"
#include "objectBillboard.h"
#include "crossUI.h"
#include "popUI.h"
#include "popUIBg.h"
#include "bowabowa.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const D3DXVECTOR2 NUMBER_SIZE = { 280.0f , 170.0f };

	const D3DXVECTOR3 LIFE_POS00 = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	const D3DXVECTOR3 LIFE_POS01 = D3DXVECTOR3(900.0f, 650.0f, 0.0f);
}

//====================================================================
//コンストラクタ
//====================================================================
CGamePlayer::CGamePlayer(int nPriority) : CPlayer(nPriority),
m_pScore(nullptr),
m_pLifeUi(nullptr),
m_pCrossUI(nullptr),
m_pCrossUIBg(nullptr),
m_pPopUIBg(nullptr),
m_pPopUI(nullptr),
m_pP_NumUI(nullptr)
{
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_fVariableSizePopUI = 0.4f;
	m_fSizePopUI = 0.0f;
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

	//体力UIの生成
	if (m_pLifeUi == nullptr)
	{
		m_pLifeUi = CLifeUi::Create();
	}

	//体力UIの生成
	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create();
	}

	//十字架アイテムの保持状態UIの生成
	if (m_pCrossUI == nullptr)
	{
		m_pCrossUI = CCrossUi::Create();
	}

	//十字架アイテムの保持状態UI背景の生成
	if (m_pCrossUIBg == nullptr)
	{
		m_pCrossUIBg = CCrossUi::Create();
	}
	
	//吹き出しUI背景の生成
	if (m_pPopUIBg == nullptr)
	{
		m_pPopUIBg = CPopUiBg::Create();
	}

	//吹き出しUIの生成
	if (m_pPopUI == nullptr)
	{
		m_pPopUI = CPopUi::Create();
	}

	//所持するUIの初期化
	InitUI();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGamePlayer::Uninit(void)
{
	// キャラクタークラスの終了（継承）
	CPlayer::Uninit();

	// スコアの削除
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}

	// ライフの削除
	if (m_pLifeUi != nullptr)
	{
		m_pLifeUi = nullptr;
	}

	// 十字架UIの削除
	if (m_pCrossUI != nullptr)
	{
		m_pCrossUI = nullptr;
	}

	// 十字架UIh背景の削除
	if (m_pCrossUIBg != nullptr)
	{
		m_pCrossUIBg = nullptr;
	}

	// 吹き出しUI背景の削除
	if (m_pPopUIBg != nullptr)
	{
		m_pPopUIBg = nullptr;
	}

	// 吹き出しUIの削除
	if (m_pPopUI != nullptr)
	{
		m_pPopUI = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CGamePlayer::Update(void)
{
	float fCrossStatePercent;

	CPlayer::Update();

	//十字架UIの更新
	if (m_pCrossUI != nullptr && m_pCrossUIBg != nullptr)
	{
		if (GetItemType() != TYPE_NONE)
		{//アイテム保持状態
			fCrossStatePercent = GetCrossStateParcent();
		}
		else
		{//アイテム非保持状態
			fCrossStatePercent = 0.0f;
		}

		//十字架UIのゲージ減少反映処理
		D3DXVECTOR2 SizeDef = m_pCrossUI->GetSizeDefault();

		m_pCrossUI->SetSize(D3DXVECTOR3(SizeDef.x, SizeDef.y * fCrossStatePercent, 0.0f));
		m_pCrossUI->SetPos(D3DXVECTOR3(m_posDefCrossUI.x, m_posDefCrossUI.y + (SizeDef.y - SizeDef.y * fCrossStatePercent) / 2, 0.0f));

		m_pCrossUI->SetAnim(D3DXVECTOR2(0.0f, 1.0 + fCrossStatePercent), D3DXVECTOR2(1.0f, 1.0f));
	}

	//吹き出しUI背景
	if (m_pPopUIBg != nullptr)
	{
		//プレイヤー位置への追従更新
		D3DXVECTOR3 PlayerPos = GetPos();
		m_pPopUIBg->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y + 100.0f, PlayerPos.z));
		
		m_fSizePopUI += m_fVariableSizePopUI;

		if (m_fSizePopUI >= 15.0f|| m_fSizePopUI<=0.0f)
		{
			m_fVariableSizePopUI = m_fVariableSizePopUI * -1;
		}
		
		m_pPopUIBg->SetHeight(75.0f + m_fSizePopUI);
		m_pPopUIBg->SetWidth(75.0f + m_fSizePopUI);

		if (GetState() == CObjectCharacter::STATE_EGG)
		{
			m_pPopUIBg->SetColorA(0.0f);
		}
		else
		{
			m_pPopUIBg->SetColorA(1.0f);
		}
		
	}

	//吹き出しUI
	if (m_pPopUI != nullptr)
	{
		//プレイヤー位置への追従更新
		D3DXVECTOR3 PlayerPos = GetPos();
		m_pPopUI->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y + 100.0f, PlayerPos.z));


		//卵状態での透明化
		if (GetState() == CObjectCharacter::STATE_EGG)
		{
			m_pPopUI->SetColorA(0.0f);
		}
		else
		{
			m_pPopUI->SetColorA(1.0f);
		}

		//表示内容の更新
		if (GetItemType() == TYPE_CROSS)
		{//アイテム保持状態
			m_pPopUI->SetpopUIType(CPopUi::TYPE_BOWABOWA);
		}
		else if(GetItemType() == TYPE_BIBLE)
		{//アイテム非保持状態
			m_pPopUI->SetpopUIType(CPopUi::TYPE_DEVILHOLE);
		}
		else
		{
			if (CBowabowa::GetList() != nullptr)
			{
				m_pPopUI->SetpopUIType(CPopUi::TYPE_CROSS);
			}
			else
			{
				m_pPopUI->SetpopUIType(CPopUi::TYPE_BIBLE);
			}
		}
	}

	//デバッグキーの処理と設定
	DebugKey();
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
		CPlayer::Death();

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

//====================================================================
// UIの初期化
//====================================================================
void CGamePlayer::InitUI()
{
	switch (GetPlayNumber())
	{
	case 0:
		if (m_pLifeUi != nullptr)
		{
			// 数字の位置
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS00.x + 200.0f, LIFE_POS00.y, LIFE_POS00.z));

			// 体力
			m_pLifeUi->SetPos(LIFE_POS00);
			m_pLifeUi->GetNumber()->SetNumber(GetLife());
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(50.0f, 40.0f, 0.0f));
		}

		//十字架UI
		if (m_pCrossUI != nullptr)
		{
			m_pCrossUI->SetPos(D3DXVECTOR3(LIFE_POS00.x+25, LIFE_POS00.y-110.0f, LIFE_POS00.z));
			m_posDefCrossUI = D3DXVECTOR2(LIFE_POS00.x+25, LIFE_POS00.y - 110.0f);
		}

		//十字架UI背景
		if (m_pCrossUIBg != nullptr)
		{
			m_pCrossUIBg->SetPos(D3DXVECTOR3(LIFE_POS00.x + 25, LIFE_POS00.y - 110.0f, LIFE_POS00.z));
			m_pCrossUIBg->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		}

		//吹き出しUI背景
		if (m_pPopUIBg != nullptr)
		{
			D3DXVECTOR3 PlayerPos=GetPos();
			m_pPopUIBg->SetPos(D3DXVECTOR3(PlayerPos.x+20.0f, PlayerPos.y - 100.0f, PlayerPos.z));
		}

		//吹き出しUI
		if (m_pPopUI != nullptr)
		{
			D3DXVECTOR3 PlayerPos = GetPos();
			m_pPopUI->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y - 100.0f, PlayerPos.z));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\1p.png");
		}

		break;

	case 1:

		if (m_pLifeUi != nullptr)
		{
			// 数字の位置
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS01.x + 200.0f, LIFE_POS01.y, LIFE_POS01.z));

			// 体力
			m_pLifeUi->SetPos(LIFE_POS01);
			m_pLifeUi->GetNumber()->SetNumber(GetLife());
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(1050.0f, 40.0f, 0.0f));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\2p.png");
		}

		break;
	}
}
//====================================================================
// デバッグボタン
//====================================================================
void CGamePlayer::DebugKey(void)
{
#ifdef _DEBUG

	int nLife = GetLife();

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//キーボードの移動処理
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		++nLife;
		m_pLifeUi->GetNumber()->SetNumber(nLife);
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		--nLife;
		m_pLifeUi->GetNumber()->SetNumber(nLife);
	}

	SetLife(nLife);

#endif // !_DEBUG
}
