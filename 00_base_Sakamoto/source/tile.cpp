//============================================
//
//	デビルホールの処理 [Tile.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Tile.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "objmeshField.h"
#include "game.h"
#include "tutorial.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	//const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(50.0f, 0.0f, 50.0f);		//当たり判定
	//const int DIRECTION = 4;	// デビルホールの最大方向
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
	m_nIdxXModel = 0;			//マテリアルの数
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_pos = INITVECTOR3;	
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	
	//for (int nCnt = 0; nCnt < DIRECTION; nCnt++)
	//{
	//	m_bSet[nCnt] = false;			//上下左右の穴が埋まっているかどうか
	//	m_pHoleKey[nCnt] = nullptr;		//上下左右の穴を埋めるポリゴン
	//}
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
CTile* CTile::Create(char* pModelName)
{
	CTile* pSample = nullptr;

	if (pSample == nullptr)
	{
		//オブジェクト2Dの生成
		pSample = new CTile();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init(pModelName)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTile::Init(char* pModelName)
{
	SetType(CObject::TYPE_DEVILHOLE);

	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

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
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

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
