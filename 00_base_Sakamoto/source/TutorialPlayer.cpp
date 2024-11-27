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

//===========================================
// 定数定義
//===========================================
namespace
{

}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CTutorialPlayer>* CTutorialPlayer::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CPlayer(nPriority)
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
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// キャラクタークラスの終了（継承）
	CPlayer::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CTutorialPlayer::Update(void)
{
	CPlayer::Update();
}

//====================================================================
//描画処理
//====================================================================
void CTutorialPlayer::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CPlayer::Draw();
}

//==========================================
//  リストの取得
//==========================================
CListManager<CTutorialPlayer>* CTutorialPlayer::GetList(void)
{
	return m_pList;
}
