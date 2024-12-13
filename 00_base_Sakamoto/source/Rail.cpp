//============================================
//
//	レールの処理 [Rail.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Rail.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "objectX.h"
#include "MapSystem.h"
#include "MapMove.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* FILE_PASS = "data\\MODEL\\rail.x"; // モデルパス
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRail>* CRail::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRail::CRail(int nPriority) : CObject(nPriority)
{
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		m_pRailModel[nCnt] = nullptr;				// レールモデル
		m_PosType[nCnt] = POSTYPE_NONE;				// レール位置の種類

	}

	// レールの方向
	for (int nCnt = 0; nCnt < POSTYPE_MAX; nCnt++)
	{
		m_bRail[nCnt] = false;
	}

	m_pPrev = nullptr;		// 前のレールへのポインタ
	m_pNext = nullptr;		// 次のレールへのポインタ

	// グリッド
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CRail::~CRail()
{

}

//====================================================================
//生成処理
//====================================================================
CRail* CRail::Create(CMapSystem::GRID grid, POSTYPE PosType0, POSTYPE PosType1)
{
	CRail* pRail = nullptr;

	if (pRail == nullptr)
	{
		//レールの生成
		pRail = new CRail();
	}

	pRail->m_Grid = grid;

	// レールの位置状態
	pRail->m_PosType[POSSTATE_FIRST] = PosType0;		// 1番目
	pRail->m_PosType[POSSTATE_SECOND] = PosType1;		// 2番目

	//オブジェクトの初期化処理
	if (FAILED(pRail->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pRail;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRail::Init()
{
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	D3DXVECTOR3 rot = INITVECTOR3;

	// レールモデル
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_PosType[nCnt] == POSTYPE_NONE) { continue; }		// レールに向きが無い場合生成しない
		else if (m_PosType[nCnt] != POSTYPE_NONE)
		{ // レールに向きがある場合

			m_pRailModel[nCnt] = CObjectX::Create(FILE_PASS);	// 生成処理
			m_pRailModel[nCnt]->SetGrid(m_Grid);				// グリッド設定
			m_pRailModel[nCnt]->SetPos(pos);					// 位置設定
			m_pRailModel[nCnt]->SetPosOld(pos);					// 前回の位置設定
			m_pRailModel[nCnt]->SetType(CObject::TYPE_RAIL);
			m_pRailModel[nCnt]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

			// 設置する向き
			rot.y = -D3DX_PI * 0.5f * m_PosType[nCnt];

			// 向き設定
			m_pRailModel[nCnt]->SetRot(rot);
			rot = INITVECTOR3;
		}
	}

	SetType(CObject::TYPE_RAIL);

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRail>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRail::Uninit(void)
{
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetDeathFlag(true);
			m_pRailModel[nCnt]->Uninit();
			m_pRailModel[nCnt] = nullptr;

		}
	}

	SetDeathFlag(true);

	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}
}

//====================================================================
//終了処理
//====================================================================
void CRail::SetNULL(void)
{
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt] = nullptr;
		}
	}
}

//====================================================================
//更新処理
//====================================================================
void CRail::Update(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;			// 向き

	D3DXVECTOR3 SlopeRot = CMapSystem::GetInstance()->GetMove()->GetDevilRot();		// マップの傾き
	

	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			//// 取得
			//pos = m_pRailModel[nCnt]->GetPos();					// 位置
			//posOld = m_pRailModel[nCnt]->GetPosOld();			// 前回の位置
			rot = m_pRailModel[nCnt]->GetRot();					// 向き

			//// 傾きによる移動量設定
			//move.x = -SlopeRot.z;
			//move.z = SlopeRot.x;

			//pos.x += move.x;
			//pos.z += move.z;

			m_pRailModel[nCnt]->SetPos(m_Grid.ToWorld());		// 位置設定
			m_pRailModel[nCnt]->SetRot(rot);					// 向き設定

			//m_pRailModel[nCnt]->SetPosOld(posOld);				// 位置
			m_pRailModel[nCnt]->Update();
		}
	}

	DebugProc::Print(DebugProc::POINT_CENTER, "Rail : %d, %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//描画処理
//====================================================================
void CRail::Draw(void)
{
	
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRail>* CRail::GetList(void)
{
	return m_pList;
}
