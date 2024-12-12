//============================================
//
//	レールマネージャー [RailManager.cpp]
//	Author:Satone Shion
//
//============================================
#include "RailManager.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* FILE_PASS = "data\\MODEL\\fireball.x"; // モデルパス
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRailManager>* CRailManager::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRailManager::CRailManager()
{
	m_pRailManagerModel[0] = nullptr;
	m_pRailManagerModel[1] = nullptr;

	m_pPrev = nullptr;		// 前のレールへのポインタ
	m_pNext = nullptr;		// 次のレールへのポインタ
}

//====================================================================
//デストラクタ
//====================================================================
CRailManager::~CRailManager()
{

}

//====================================================================
//生成処理
//====================================================================
CRailManager* CRailManager::Create()
{
	CRailManager* pSample = nullptr;

	if (pSample == nullptr)
	{
		//オブジェクト2Dの生成
		pSample = new CRailManager();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailManager::Init()
{

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRailManager>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt]->SetDeathFlag(true);
		}
	}

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
void CRailManager::SetNULL(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt] = nullptr;
		}
	}
}

//====================================================================
//更新処理
//====================================================================
void CRailManager::Update(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CRailManager::Draw(void)
{

}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRailManager>* CRailManager::GetList(void)
{
	return m_pList;
}
