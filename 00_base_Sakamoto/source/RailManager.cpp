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
#include "MapSystem.h"
#include "Rail.h"

//==========================================
//  定数定義
//==========================================
namespace
{

}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRailManager>* CRailManager::m_pList = nullptr;	// オブジェクトリスト
std::vector<CMapSystem::GRID> CRailManager::m_GridPos = {};		// レールの位置を保持

//====================================================================
//コンストラクタ
//====================================================================
CRailManager::CRailManager()
{
	
}

//====================================================================
//デストラクタ
//====================================================================
CRailManager::~CRailManager()
{
	m_GridPos.clear();
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailManager::Init(void)
{
	int nMax = m_GridPos.size();
	int nCnt = 0;

	while (nCnt < nMax)
	{ // グリッドの数分まわす

		// 何もない状態にする
		CRail::POSTYPE PosType[CRail::POSSTATE_MAX] = { CRail::POSTYPE::POSTYPE_NONE, CRail::POSTYPE::POSTYPE_NONE };

		//モデルの向き設定
		SetRot(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		// レールの生成
		CRail::Create(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		nCnt++;
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailManager::Uninit(void)
{
	m_GridPos.clear();
}

//====================================================================
//更新処理
//====================================================================
void CRailManager::Update(void)
{
	
}

//====================================================================
//描画処理
//====================================================================
void CRailManager::Draw(void)
{

}

//====================================================================
// レールの設定
//====================================================================
void CRailManager::Set(CMapSystem::GRID& grid)
{
	// レールの位置を保持
	m_GridPos.push_back(grid);
}

//====================================================================
// モデルの向きの設定
//====================================================================
void CRailManager::SetRot(CMapSystem::GRID& grid, CRail::POSTYPE& PosType0, CRail::POSTYPE& PosType1)
{
	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // マップシステムのインスタンスを取得
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// マップの横幅
	MaxGrid.z = pMapSystem->GetHeightMax(); // マップの立幅
	bool bRail[CRail::POSTYPE_MAX];					// レールの置ける位置の判定

	/* 自身の隣接４マスのグリッド */
	CMapSystem::GRID nNumber[CRail::POSTYPE_MAX];	// 4方向の隣接するグリッド

	for (int i = 0; i < CRail::POSTYPE_MAX;++i)
	{
		// 隣接グリッドの設定
		nNumber[i] = CMapSystem::GRID(grid.x + (int)sinf(D3DX_PI * 0.5f * i), grid.z + (int)cosf(D3DX_PI * 0.5f * i));
		
		// グリッド番号の丸め込み
		nNumber[i].x = useful::RangeNumber(MaxGrid.x, 0, nNumber[i].x);
		nNumber[i].z = useful::RangeNumber(MaxGrid.z, 0, nNumber[i].z);

		// 配置情報の更新
		bRail[i] = false;
		bRail[i] = pMapSystem->GetRailGritBool(nNumber[i]);
	}

	// レールの配置場所設定
	for (int nCnt = 0; nCnt < CRail::POSTYPE_MAX; nCnt++)
	{
		if (!bRail[nCnt]) { continue; }
		else if (bRail[nCnt])
		{ // 配置可能な場合

			// 場所設定
			if (PosType0 == CRail::POSTYPE_NONE)
			{ // 1つ目
				PosType0 = (CRail::POSTYPE)nCnt;
			}
			else if (PosType1 == CRail::POSTYPE_NONE)
			{ // 2つ目
				PosType1 = (CRail::POSTYPE)nCnt;
			}
		}
	}
}