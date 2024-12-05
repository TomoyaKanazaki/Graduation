//============================================
//
//	床モデルの処理 [tile.cpp]
//	Author:Satone Shion
//
//============================================
#include "tile.h"
#include "renderer.h"
#include "manager.h"
#include "objmeshField.h"
#include "game.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const char* FILE_PASS = "data\\MODEL\\00_floor.x"; // モデルパス
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CTile>* CTile::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CTile::CTile(int nPriority) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CTile::CTile(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(INITVECTOR3);
	SetPos(INITVECTOR3);
	m_Grid.x = gridCenter.x;	// グリッドの位置X
	m_Grid.z = gridCenter.z;	// グリッドの位置Z
}

//====================================================================
//デストラクタ
//====================================================================
CTile::~CTile()
{

}

//====================================================================
//生成処理
//====================================================================
CTile* CTile::Create(CMapSystem::GRID gridCenter)
{
	CTile* pSample = nullptr;

	if (pSample == nullptr)
	{
		// 床の生成
		pSample = new CTile(3, gridCenter);
	}

	// 床の初期化処理
	if (FAILED(pSample->Init(FILE_PASS)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTile::Init(const char* pModelName)
{
	SetType(CObject::TYPE_TILE);

	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 位置設定
	CObjectX::SetPos(m_Grid.ToWorld());

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CTile>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTile::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CTile::Update(void)
{
	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CTile::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CTile>* CTile::GetList(void)
{
	return m_pList;
}
