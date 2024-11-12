//============================================
//
//	ボワボワの処理 [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "bowabowa.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "score.h"
#include "player.h"
#include "sound.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\Testbowabowa.x"; // モデルパス
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CBowabowa>* CBowabowa::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CBowabowa::CBowabowa(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//デストラクタ
//====================================================================
CBowabowa::~CBowabowa()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CBowabowa::Init()
{
	// 親クラスの初期化
	if(FAILED(CItem::Init(MODEL_PASS))){ assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_BOWABOWA);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBowabowa>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CBowabowa::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了処理
	CItem::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CBowabowa::Update(void)
{
	// 親クラスの更新処理
	CItem::Update();
}

//====================================================================
//描画処理
//====================================================================
void CBowabowa::Draw(void)
{
	// 親クラスの描画処理
	CItem::Draw();
}

//==========================================
// ヒット処理
//==========================================
bool CBowabowa::Hit(CPlayer* pPlayer)
{
	// 十字架を持ってない場合関数を抜ける
	if (pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return false;
	}

	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET_BOWA);

	// 削除
	Uninit();
	return true;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CBowabowa>* CBowabowa::GetList(void)
{
	return m_pList;
}
