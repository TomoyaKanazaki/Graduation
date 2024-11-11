//========================================================================================
//
// マップシステム[MapSystem.cpp]
// Author: 坂本　翔唯
//
//========================================================================================
#include "MapSystem.h"
#include "renderer.h"
#include "game.h"
#include "Devil.h"

// 定数定義
namespace
{
	float GRID_SIZE = 100.0f;	// グリッドのサイズ
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// 横の当たり判定
}

//静的メンバ変数宣言
CMapSystem* CMapSystem::pMapSystem = nullptr;
bool CMapSystem::m_nMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};

//====================================================================
//コンストラクタ
//====================================================================
CMapSystem::CMapSystem()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			m_nMapGrit[nCntW][nCntH] = false;
		}
	}
	m_WightMax = NUM_WIGHT;
	m_HeightMax = NUM_HEIGHT;
	m_MapPos = D3DXVECTOR3((m_WightMax * 0.5f) * -100.0f, 0.0f, (m_HeightMax * 0.5f) * 100.0f);
	m_InitPos = m_MapPos;
	m_fGritSize = GRID_SIZE;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
}

//====================================================================
//デストラクタ
//====================================================================
CMapSystem::~CMapSystem()
{

}

//====================================================================
//インスタンス取得
//====================================================================
CMapSystem* CMapSystem::GetInstance(void)
{
	if (pMapSystem == nullptr)
	{
		pMapSystem = new CMapSystem;
	}
	return pMapSystem;
}

//====================================================================
//マップシステムの初期化処理
//====================================================================
void CMapSystem::Init()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			pMapSystem->m_nMapGrit[nCntW][nCntH] = false;
		}
	}

	m_MapPos = D3DXVECTOR3((m_WightMax * 0.5f) * -100.0f, 0.0f, (m_HeightMax * 0.5f) * 100.0f);
	m_InitPos = m_MapPos;
}

//====================================================================
//マップシステムの終了処理
//====================================================================
void CMapSystem::Uninit(void)
{
	if (pMapSystem != nullptr)
	{
		delete pMapSystem;
		pMapSystem = nullptr;
	}
}

//====================================================================
//マップシステムの更新処理
//====================================================================
void CMapSystem::Update(void)
{

}

//====================================================================
//マップシステムの描画処理
//====================================================================
void CMapSystem::Draw(void)
{

}

//====================================================================
// グリットの開始位置取得
//====================================================================
D3DXVECTOR3 CMapSystem::GetStartGritPos(float Wight, float Height)
{
	D3DXVECTOR3 Pos;

	if (Wight < 0 || Wight >= (float)m_WightMax ||
		Height < 0 || Height >= (float)m_HeightMax)
	{
		return Pos;
	}

	//　グリットの位置にエフェクトを表示
	Pos.x = m_InitPos.x + (Wight * m_fGritSize);
	Pos.y = 0.0f;
	Pos.z = m_InitPos.z - (Height * m_fGritSize);

	return Pos;
}

//====================================================================
// グリットの位置取得
//====================================================================
D3DXVECTOR3 CMapSystem::GetGritPos(int Wight, int Height)
{
	D3DXVECTOR3 Pos;

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();

	// グリット番号が最大値以上や最小値以下の時、範囲内に納める処理
	Wight = useful::RangeNumber(m_WightMax, 0, Wight);
	Height = useful::RangeNumber(m_HeightMax, 0, Height);

	// グリットの横番号の位置を設定する
	Pos.x = m_MapPos.x + (Wight * m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (Pos.x > DevilPos.x + (m_MapSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
	}

	// 高さの位置を設定する
	Pos.y = 0.0f;

	// グリットの縦番号の位置を設定する
	Pos.z = m_MapPos.z - (Height * m_fGritSize);

	//境界線の外側にグリットがある場合反対側に移動させる
	if (Pos.z < DevilPos.z - (m_MapSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}

//==========================================
//  グリッドを算出
//==========================================
CMapSystem::GRID CMapSystem::CalcGrid(const D3DXVECTOR3& pos)
{
	// 算出に使用する変数
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();
	GRID grid;

	// x座標の算出
	for (int i = 0; i < m_WightMax; i++)
	{
		float fCountPosX = m_MapPos.x + (i * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (pos.x < fCountPosX + (m_fGritSize * 0.5f) &&
			pos.x >= fCountPosX - (m_fGritSize * 0.5f))
		{
			grid.x = i;
		}
	}

	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (pos.z < fCountPosZ + (m_fGritSize * 0.5f) &&
			pos.z >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			grid.z = i;
		}
	}

	return grid;
}
