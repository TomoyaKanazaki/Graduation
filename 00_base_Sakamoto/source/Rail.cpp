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

using namespace Rail;

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRail>* CRail::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRail::CRail(int nPriority) : CObject(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
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

	//オブジェクトの初期化処理
	if (FAILED(pRail->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	pRail->m_Grid = grid;
	pRail->m_PosType[0] = PosType0;
	pRail->m_PosType[1] = PosType1;


	return pRail;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRail::Init()
{
	// 初期化処理
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
	{
		m_pRailModel[nCnt]->Init(FILE_PASS);
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
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
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
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
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
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CRail::Draw(void)
{

}

//====================================================================
//前のモデルの設定
//====================================================================
void CRail::PrevSet(POSTYPE Set)
{
	//引数で設定した方向にレールを置く
	m_bRail[Set] = true;

	//真ん中からのレールを設置する
	if (m_pRailModel[0] == nullptr)
	{
		m_pRailModel[0] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[0]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailModel[0]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailModel[0]->SetMultiMatrix(true);

		switch (Set)
		{
		case CRail::POSTYPE_UP:	// 上
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			break;
		case CRail::POSTYPE_DOWN:	// 下
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			break;
		case CRail::POSTYPE_LEFT:	// 左
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			break;
		case CRail::POSTYPE_RIGHT:	// 右
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			break;
		default:
			break;
		}
	}
}

//====================================================================
//次のモデルの設定
//====================================================================
void CRail::NextSet(POSTYPE Set)
{
	//引数で設定した方向にレールを置く
	m_bRail[Set] = true;
	m_bNextNumber = Set;

	//真ん中までのレールを設置する
	if (m_pRailModel[1] == nullptr)
	{
		m_pRailModel[1] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[1]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailModel[1]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailModel[1]->SetMultiMatrix(true);

		//伸ばす前のレールの位置を取得する
		int nMapWight = GetWightNumber();
		int nMapHeight = GetHeightNumber();

		//引数で設定した方向にレールの向き、番号を設定
		switch (Set)
		{
		case CRail::POSTYPE_UP:	// 上
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			Set = POSTYPE_DOWN;
			nMapHeight = GetHeightNumber() - 1;
			break;

		case CRail::POSTYPE_DOWN:	// 下
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			Set = POSTYPE_UP;
			nMapHeight = GetHeightNumber() + 1;
			break;

		case CRail::POSTYPE_LEFT:	// 左
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			Set = POSTYPE_RIGHT;
			nMapWight = GetWightNumber() - 1;
			break;

		case CRail::POSTYPE_RIGHT:	// 右
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			Set = POSTYPE_LEFT;
			nMapWight = GetWightNumber() + 1;
			break;

		default:
			break;
		}

		//次のレールを設定する
		/*m_pNext = CRail::Create();
		m_pNext->SetWightNumber(nMapWight);
		m_pNext->SetHeightNumber(nMapHeight);
		m_pNext->SetPrevRail(this);
		m_pNext->PrevSet(Set);*/
	}
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRail>* CRail::GetList(void)
{
	return m_pList;
}
