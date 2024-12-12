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

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailManager::Init(CMapSystem::GRID& grid)
{
	// レールの位置を保持
	m_GridPos.push_back(grid);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailManager::Uninit(void)
{
	
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
void CRailManager::Set(void)
{
	// 何もない状態にする
	CRail::POSTYPE PosType[CRail::POSSTATE_MAX];

	for (int nCnt = 0; nCnt < CRail::POSSTATE_MAX; nCnt++)
	{
		PosType[nCnt] = CRail::POSTYPE_NONE;
	}

	int nMax = m_GridPos.size();
	int nCnt = 0;

	while (nCnt <= nMax)
	{ // グリッドの数分まわす

		//モデルの向き設定
		SetRot(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		// レールの生成
		CRail::Create(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);
	}
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

	/* 自身の隣接４マスのグリッド */
	int nNumber[CRail::POSTYPE_MAX];	// 4方向の隣接するグリッド
	nNumber[CRail::POSTYPE_LEFT] = grid.x - 1;		// 左
	nNumber[CRail::POSTYPE_RIGHT] = grid.x + 1;		// 右
	nNumber[CRail::POSTYPE_UP] = grid.z - 1;		// 上
	nNumber[CRail::POSTYPE_DOWN] = grid.z + 1;		// 下

	// グリッド位置の補正
	nNumber[CRail::POSTYPE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[CRail::POSTYPE_LEFT]);
	nNumber[CRail::POSTYPE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[CRail::POSTYPE_RIGHT]);
	nNumber[CRail::POSTYPE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[CRail::POSTYPE_UP]);
	nNumber[CRail::POSTYPE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[CRail::POSTYPE_DOWN]);

	// 隣接４マスがレール配置可能か判断する
	m_bRail[CRail::POSTYPE_LEFT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_LEFT], grid.z));
	m_bRail[CRail::POSTYPE_RIGHT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_RIGHT], grid.z));
	m_bRail[CRail::POSTYPE_UP] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_UP]));
	m_bRail[CRail::POSTYPE_DOWN] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_DOWN]));

	// レールの配置場所設定
	for (int nCnt = CRail::POSTYPE_UP; nCnt < CRail::POSTYPE_MAX; nCnt++)
	{
		if (m_bRail[nCnt])
		{ // 配置可能な場合

			// 場所設定
			if (PosType0 == CRail::POSTYPE_NONE)
			{ // 1つ目
				switch (nCnt)
				{
				case CRail::POSTYPE_UP:		// 上
					PosType0 = CRail::POSTYPE_UP;
					break;

				case CRail::POSTYPE_DOWN:	// 下
					PosType0 = CRail::POSTYPE_DOWN;
					break;

				case CRail::POSTYPE_LEFT:	// 左
					PosType0 = CRail::POSTYPE_LEFT;
					break;

				case CRail::POSTYPE_RIGHT:	// 右
					PosType0 = CRail::POSTYPE_RIGHT;
					break;

				default:
					break;
				}

			}
			else if (PosType1 == CRail::POSTYPE_NONE)
			{ // 2つ目
				switch (nCnt)
				{
				case CRail::POSTYPE_UP:		// 上
					PosType1 = CRail::POSTYPE_UP;
					break;

				case CRail::POSTYPE_DOWN:	// 下
					PosType1 = CRail::POSTYPE_DOWN;
					break;

				case CRail::POSTYPE_LEFT:	// 左
					PosType1 = CRail::POSTYPE_LEFT;
					break;

				case CRail::POSTYPE_RIGHT:	// 右
					PosType1 = CRail::POSTYPE_RIGHT;
					break;

				default:
					break;
				}
			}

		}
	}
}