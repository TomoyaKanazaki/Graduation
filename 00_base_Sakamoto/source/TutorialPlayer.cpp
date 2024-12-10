//============================================
//
//	チュートリアルのプレイヤー [TutorialPlayer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "TutorialPlayer.h"
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
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// チェックマーカーテクスチャ

	const D3DXVECTOR3 MARKER_TEX_POS = D3DXVECTOR3(50.0f, 160.0f, 0.0f);	// マーカー位置
	const D3DXVECTOR3 MARKER_TEX_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// マーカーサイズ

}

//===========================================
// 静的メンバ変数宣言
//===========================================

//====================================================================
//コンストラクタ
//====================================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CPlayer(nPriority),
m_pCheckMaker(nullptr)
{

}

//====================================================================
//デストラクタ
//====================================================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//====================================================================
//生成処理
//====================================================================
CTutorialPlayer* CTutorialPlayer::Create(int PlayNumber)
{
	CTutorialPlayer* pPlayer = new CTutorialPlayer();

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
HRESULT CTutorialPlayer::Init(int PlayNumber)
{
	CPlayer::Init(PlayNumber);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTutorialPlayer::Uninit(void)
{
	// キャラクタークラスの終了（継承）
	CPlayer::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CTutorialPlayer::Update(void)
{
	CPlayer::Update();

	switch (GetItemType())
	{
	case TYPE_CROSS:

		if (m_pCheckMaker == nullptr)
		{// チェックマーカー生成
			m_pCheckMaker = CObject2D::Create();
		}
		if (m_pCheckMaker != nullptr)
		{// テクスチャ生成・位置・サイズ設定
			m_pCheckMaker->SetTexture(CHECK_MARKER_TEX);
			m_pCheckMaker->SetPos(MARKER_TEX_POS);
			m_pCheckMaker->SetSize(MARKER_TEX_SIZE);
		}

		break;

	case TYPE_BIBLE:

		if (m_pCheckMaker == nullptr)
		{// チェックマーカー生成
			m_pCheckMaker = CObject2D::Create();
		}
		if (m_pCheckMaker != nullptr)
		{// テクスチャ生成・位置・サイズ設定
			m_pCheckMaker->SetTexture(CHECK_MARKER_TEX);
			m_pCheckMaker->SetPos(D3DXVECTOR3(50.0f, 0.0f, 0.0f));
			m_pCheckMaker->SetSize(MARKER_TEX_SIZE);
		}

		break;

	default:
		break;
	}
}

//====================================================================
//描画処理
//====================================================================
void CTutorialPlayer::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CPlayer::Draw();
}

//====================================================================
//ダメージ処理
//====================================================================
void CTutorialPlayer::Death(void)
{
	if (GetState() != STATE_EGG && GetState() != STATE_DEATH)
	{
		CPlayer::Death();

		// 聖書を所持しているときにその場に聖書を落とす
		if (GetItemType() == TYPE_BIBLE)
		{
			// 聖書生成
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GetGrid()));
		}

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

		// アイテムを所持していない状態にする
		SetItemType(TYPE_NONE);
	}
}