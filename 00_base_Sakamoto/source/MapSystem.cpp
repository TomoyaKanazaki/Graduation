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

// マクロ定義
#define GRIT_SIZE (100.0f)

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
	m_fGritSize = GRIT_SIZE;
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

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 DevilSize = CGame::GetDevil()->GetDevilSize();

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
	D3DXVECTOR3 DevilSize = CGame::GetDevil()->GetDevilSize();

	if (Wight < 0 || Wight >= m_WightMax ||
		Height < 0 || Height >= m_HeightMax)
	{
		return Pos;
	}

	//　グリットの位置にエフェクトを表示
	Pos.x = m_MapPos.x + (Wight * m_fGritSize);

	if (Pos.x > DevilPos.x + (DevilSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (DevilSize.x * 2.0f)) - m_fGritSize;
	}

	Pos.y = 0.0f;

	Pos.z = m_MapPos.z - (Height * m_fGritSize);

	if (Pos.z < DevilPos.z - (DevilSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (DevilSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}

//====================================================================
// グリットの横番号取得
//====================================================================
int CMapSystem::GetGritWightNumber(float PosX)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();
	D3DXVECTOR3 DevilSize = pDevil->GetDevilSize();

	for (int nCntW = 0; nCntW < m_WightMax; nCntW++)
	{
		float fCountPosX = m_MapPos.x + (nCntW * m_fGritSize);

		if (fCountPosX > DevilPos.x + (DevilSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (DevilSize.x * 2.0f)) - m_fGritSize;
		}

		if (PosX < fCountPosX + (m_fGritSize * 0.5f) &&
			PosX >= fCountPosX - (m_fGritSize * 0.5f))
		{
			return nCntW;
		}
	}

	return -1;
}

//====================================================================
// グリットの縦番号取得
//====================================================================
int CMapSystem::GetGritHeightNumber(float PosZ)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();
	D3DXVECTOR3 DevilSize = pDevil->GetDevilSize();

	for (int nCntH = 0; nCntH < m_HeightMax; nCntH++)
	{
		float fCountPosZ = m_MapPos.z - (nCntH * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (DevilSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (DevilSize.z * 2.0f)) + m_fGritSize;
		}

		if (PosZ < fCountPosZ + (m_fGritSize * 0.5f) &&
			PosZ >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			return nCntH;
		}
	}
	return -1;
}
