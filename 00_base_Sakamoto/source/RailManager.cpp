//============================================
//
//	レールマネージャー [RailManager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RailManager.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "objectX.h"
#include "MapSystem.h"
#include "game.h"
#include "objmeshField.h"

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
CRailManager::CRailManager(int nPriority) : CObjectX(nPriority)
{
	m_pRailManagerModel[0] = nullptr;
	m_pRailManagerModel[1] = nullptr;

	for (int nCnt = 0; nCnt < RAIL_POS_MAX; nCnt++)
	{
		m_bRailManager[nCnt] = false;
	}

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
	CObjectX::Init(FILE_PASS);

	SetType(CObject::TYPE_RAIL);

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

	SetDeathFlag(true);

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
//前のモデルの設定
//====================================================================
void CRailManager::PrevSet(RAIL_POS Set)
{
	//引数で設定した方向にレールを置く
	m_bRailManager[Set] = true;

	//真ん中からのレールを設置する
	if (m_pRailManagerModel[0] == nullptr)
	{
		m_pRailManagerModel[0] = CObjectX::Create("data\\MODEL\\TestRailManager.x");
		m_pRailManagerModel[0]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailManagerModel[0]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailManagerModel[0]->SetMultiMatrix(true);

		switch (Set)
		{
		case CRailManager::RAIL_POS_UP:	// 上
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			break;
		case CRailManager::RAIL_POS_DOWN:	// 下
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			break;
		case CRailManager::RAIL_POS_LEFT:	// 左
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			break;
		case CRailManager::RAIL_POS_RIGHT:	// 右
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			break;
		default:
			break;
		}
	}
}

//====================================================================
//次のモデルの設定
//====================================================================
void CRailManager::NextSet(RAIL_POS Set)
{
	//引数で設定した方向にレールを置く
	m_bRailManager[Set] = true;
	m_bNextNumber = Set;

	//真ん中までのレールを設置する
	if (m_pRailManagerModel[1] == nullptr)
	{
		m_pRailManagerModel[1] = CObjectX::Create("data\\MODEL\\TestRailManager.x");
		m_pRailManagerModel[1]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailManagerModel[1]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailManagerModel[1]->SetMultiMatrix(true);

		//伸ばす前のレールの位置を取得する
		int nMapWight = GetWightNumber();
		int nMapHeight = GetHeightNumber();

		//引数で設定した方向にレールの向き、番号を設定
		switch (Set)
		{
		case CRailManager::RAIL_POS_UP:	// 上
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			Set = RAIL_POS_DOWN;
			nMapHeight = GetHeightNumber() - 1;
			break;

		case CRailManager::RAIL_POS_DOWN:	// 下
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			Set = RAIL_POS_UP;
			nMapHeight = GetHeightNumber() + 1;
			break;

		case CRailManager::RAIL_POS_LEFT:	// 左
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			Set = RAIL_POS_RIGHT;
			nMapWight = GetWightNumber() - 1;
			break;

		case CRailManager::RAIL_POS_RIGHT:	// 右
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			Set = RAIL_POS_LEFT;
			nMapWight = GetWightNumber() + 1;
			break;

		default:
			break;
		}

		//次のレールを設定する
		m_pNext = CRailManager::Create();
		m_pNext->SetWightNumber(nMapWight);
		m_pNext->SetHeightNumber(nMapHeight);
		m_pNext->SetPrevRailManager(this);
		m_pNext->PrevSet(Set);
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRailManager>* CRailManager::GetList(void)
{
	return m_pList;
}
