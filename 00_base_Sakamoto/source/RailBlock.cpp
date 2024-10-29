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
#include "debugproc.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float RAIL_WIGHT = 5.0f;	//レール幅
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
		CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), false);
		SetWightNumber(CMapSystem::GetInstance()->GetGritWightNumber(Pos.x));
		CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), true);

		RailCheck();
	}

	if (GetHeightNumber() != CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z))
	{
		CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), false);
		SetHeightNumber(CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z));
		CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), true);

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
	D3DXVECTOR3 TestPos = INITVECTOR3;

	D3DXVECTOR3 SlopeMove = INITVECTOR3;
	D3DXVECTOR3 SlopeRot = CGame::GetDevil()->GetDevilRot();
	D3DXVECTOR3 GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());
	D3DXVECTOR3 GritDistance = *Pos - GritPos;	//グリットの中心とした時の相対位置、差分

	SlopeMove.x = -SlopeRot.z * 10.0f;
	SlopeMove.z = SlopeRot.x * 10.0f;

	if (useful::CollisionCircle(GritPos, D3DXVECTOR3(Pos->x, GritPos.y, Pos->z), RAIL_WIGHT) == true)
	{
		Pos->x += SlopeMove.x;
		Pos->z += SlopeMove.z;
	}
	else
	{
		if (GritPos.x - Pos->x >= -5.0f && GritPos.x - Pos->x <= RAIL_WIGHT)
		{
			Pos->z += SlopeMove.z;
		}
		else
		{
			Pos->z = GritPos.z;
		}

		if (GritPos.z - Pos->z >= -5.0f && GritPos.z - Pos->z <= RAIL_WIGHT)
		{
			Pos->x += SlopeMove.x;
		}
		else
		{
			Pos->x = GritPos.x;
		}
	}

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

	if (CMapSystem::GetInstance()->GetGritWightNumber(Pos->x) == -1)
	{
		if (Pos->x > 0.0f)	//右のグリット外に出たとき
		{
			if (SlopeMove.x > 0.0f && bMoveOK[3])	//X軸の動きが[+]で右にレールが存在する時
			{
				//１つ右のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber() + 1, GetHeightNumber());
			}
			else
			{
				// 現在のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());
			}
		}
		else if (Pos->x < 0.0f)	//左のグリット外に出たとき
		{
			if (SlopeMove.x < 0.0f && bMoveOK[2])	//X軸の動きが[-]で左にレールが存在する時
			{
				//１つ右のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber() - 1, GetHeightNumber());
			}
			else
			{
				// 現在のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());
			}
		}
			Pos->x = GritPos.x;
	}

	if (CMapSystem::GetInstance()->GetGritHeightNumber(Pos->z) == -1)
	{
		if (Pos->z > 0.0f)	//右のグリット外に出たとき
		{
			if (SlopeMove.z > 0.0f && bMoveOK[0])	//Z軸の動きが[+]で右にレールが存在する時
			{
				//１つ右のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber() - 1);
			}
			else
			{
				// 現在のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());
			}
		}
		else if (Pos->z < 0.0f)	//左のグリット外に出たとき
		{
			if (SlopeMove.z < 0.0f && bMoveOK[1])	//Z軸の動きが[-]で左にレールが存在する時
			{
				//１つ右のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber() + 1);
			}
			else
			{
				// 現在のグリットに移動
				GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());
			}
		}
		Pos->z = GritPos.z;
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "[レールブロック]位置 %f : %f : %f\n", Pos->x, Pos->y, Pos->z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[レールブロック]横番号 %d \n", GetWightNumber());
	DebugProc::Print(DebugProc::POINT_LEFT, "[レールブロック]縦番号 %d \n", GetHeightNumber());
	DebugProc::Print(DebugProc::POINT_LEFT, "[レールブロック]差分 %f : %f \n", GritDistance.x, GritDistance.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[テストブロック]差分 %f : %f \n", TestPos.x, TestPos.z);
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

	int nMax = 5;
	int nData[5];

	nData[0] = 3;
	nData[1] = 3;
	nData[2] = 1;
	nData[3] = 1;
	nData[4] = 1;

	CRail* pRail = m_pTop->GetNextRail();

	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		pRail->NextSet((CRail::RAIL_POS)nData[nCnt]);

		pRail = pRail->GetNextRail();
	}

	m_pCur = pRail;
}