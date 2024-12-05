//============================================
//
//	シグナルマネージャー [signalmanager.cpp]
//	Author:morikawa shunya
//
//============================================
#include "signalmanager.h"
#include "useful.h"
#include "MapSystem.h"
#include "game.h"
#include "renderer.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float BASE_Y = 50.0f; // 高さ
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CSignalManager>* CSignalManager::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CSignalManager::CSignalManager(int nPriority)
{
	m_nStateCount = 0;			// 状態管理
	m_nStateNum = 0;
	m_fRot = 0.0f;
	m_pDevil = nullptr;
}

//====================================================================
// デストラクタ
//====================================================================
CSignalManager::~CSignalManager()
{

}

//==========================================
//  リストの取得
//==========================================
CListManager<CSignalManager>* CSignalManager::GetList(void)
{
	return m_pList;
}

//====================================================================
// 生成
//====================================================================
CSignalManager* CSignalManager::Create()
{
	CSignalManager* pSignalmanager = nullptr;

	if (pSignalmanager == nullptr)
	{
		//プレイヤーの生成
		pSignalmanager = new CSignalManager();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSignalmanager->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSignalmanager;
}


//====================================================================
// 初期化
//====================================================================
HRESULT CSignalManager::Init(void)
{
	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSignalManager>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	m_pDevil = CDevil::Create();

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CSignalManager::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}
}

//====================================================================
// 更新
//====================================================================
void CSignalManager::Update()
{

}

//====================================================================
// ムーブ設定
//====================================================================
void CSignalManager::SetMove(int nType)
{
	
}