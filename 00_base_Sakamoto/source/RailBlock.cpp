//============================================
//
//	レールブロックの処理 [RailBlock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RailBlock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "Rail.h"
#include "game.h"
#include "Devil.h"
#include "MapSystem.h"

//==========================================
//  定数定義
//==========================================
namespace
{

}

//====================================================================
//コンストラクタ
//====================================================================
CRailBlock::CRailBlock(int nPriority) :CCubeBlock(nPriority)
{
	m_pTop = nullptr;		// 先頭のレールへのポインタ
	m_pCur = nullptr;		// 最後尾のレールへのポインタ

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		bMoveOK[nCnt] = false;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CRailBlock::~CRailBlock()
{

}

//====================================================================
//生成処理
//====================================================================
CRailBlock* CRailBlock::Create(int nMapWight, int nMapHeight)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//オブジェクト3Dの生成
		pObjectBlock = new CRailBlock();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObjectBlock->Init(nMapWight, nMapHeight)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailBlock::Init(int nMapWight, int nMapHeight)
{
	SetWightNumber(nMapWight);
	SetHeightNumber(nMapHeight);

	StartWightNumber = nMapWight;
	StartHeightNumber = nMapHeight;

	CCubeBlock::Init();

	SetTexture("data\\TEXTURE\\Wood001.png");

	SetType(TYPE_RAILBLOCK);

	RailSet();

	RailCheck();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailBlock::Uninit(void)
{
	CCubeBlock::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();

	Move(&Pos);

	if (GetWightNumber() != CMapSystem::GetInstance()->GetGritWightNumber(Pos.x))
	{
		SetWightNumber(CMapSystem::GetInstance()->GetGritWightNumber(Pos.x));
		RailCheck();
	}

	if (GetHeightNumber() != CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z))
	{
		SetHeightNumber(CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z));
		RailCheck();
	}

	SetPos(Pos);

	CCubeBlock::Update();
}

//====================================================================
//描画処理
//====================================================================
void CRailBlock::Draw(void)
{
	CCubeBlock::Draw();
}

//====================================================================
//傾き時の移動処理
//====================================================================
void CRailBlock::Move(D3DXVECTOR3* Pos)
{
	D3DXVECTOR3 SlopeMove = INITVECTOR3;
	D3DXVECTOR3 SlopeRot = CGame::GetDevil()->GetDevilRot();
	D3DXVECTOR3 GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());

	SlopeMove.x = SlopeRot.x * 10.0f;
	SlopeMove.z = SlopeRot.z * 10.0f;

	Pos->x -= SlopeMove.z;
	Pos->z += SlopeMove.x;

	// 上
	if (!bMoveOK[0])
	{
		if (Pos->z > GritPos.z)
		{
			Pos->z = GritPos.z;
		}
	}

	// 下
	if (!bMoveOK[1])
	{
		if (Pos->z < GritPos.z)
		{
			Pos->z = GritPos.z;
		}
	}

	// 左
	if (!bMoveOK[2])
	{
		if (Pos->x < GritPos.x)
		{
			Pos->x = GritPos.x;
		}
	}

	// 右
	if (!bMoveOK[3])
	{
		if (Pos->x > GritPos.x)
		{
			Pos->x = GritPos.x;
		}
	}
}

//====================================================================
//レールの有無を設定する
//====================================================================
void CRailBlock::RailCheck(void)
{
	CRail* pRail = m_pTop;
	int WightNumber = GetWightNumber();
	int HeightNumber = GetHeightNumber();

	while (1)
	{
		if (WightNumber == pRail->GetWightNumber() &&
			HeightNumber == pRail->GetHeightNumber())
		{
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				bMoveOK[nCnt] = pRail->GetRailOK(nCnt);
			}
			return;
		}

		if (pRail == m_pCur)
		{
			break;
		}

		pRail = pRail->GetNextRail();
	}
}

//====================================================================
//レールの設置処理
//====================================================================
void CRailBlock::RailSet(void)
{
	m_pTop = CRail::Create();
	m_pTop->SetWightNumber(GetWightNumber());
	m_pTop->SetHeightNumber(GetHeightNumber());
	m_pTop->NextSet(CRail::RAIL_POS_RIGHT);

	int nMax = 4;
	int nData[4];

	nData[0] = 0;
	nData[1] = 3;
	nData[2] = 1;
	nData[3] = 3;

	CRail* pRail = m_pTop->GetNextRail();

	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		pRail->NextSet((CRail::RAIL_POS)nData[nCnt]);

		pRail = pRail->GetNextRail();
	}

	m_pCur = pRail;
}