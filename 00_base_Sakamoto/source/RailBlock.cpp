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
#include "player.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float RAIL_WIGHT = 5.0f;	//レール幅
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRailBlock>* CRailBlock::m_pList = nullptr; // オブジェクトリスト

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
CRailBlock* CRailBlock::Create(int nMapWight, int nMapHeight, bool Edit, int Max, int* nMove)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//オブジェクト3Dの生成
		pObjectBlock = new CRailBlock();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObjectBlock->Init(nMapWight, nMapHeight, Edit, Max, nMove)))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailBlock::Init(int nMapWight, int nMapHeight, bool Edit, int Max, int* nMove)
{
	SetWightNumber(nMapWight);
	SetHeightNumber(nMapHeight);

	StartWightNumber = nMapWight;
	StartHeightNumber = nMapHeight;

	CCubeBlock::Init();

	SetPos(CMapSystem::GetInstance()->GetGritPos(StartWightNumber, StartHeightNumber));

	SetTexture("data\\TEXTURE\\Wood001.png");

	SetType(TYPE_RAILBLOCK);

	if (Edit == false)
	{
		RailSet(Max, nMove);

		RailCheck();
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRailBlock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailBlock::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	//自身が所持するレールを全て削除する
	CRail* pRail = m_pTop;
	while (1)
	{
		if (pRail != nullptr)
		{
			pRail->Uninit();

			pRail = pRail->GetNextRail();
		}
		else
		{
			break;
		}
	}
	m_pTop = nullptr;
	m_pCur = nullptr;

	CCubeBlock::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PosOld = GetPos();

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
	SetPosOld(PosOld);

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

	//傾きによる移動量設定
	SlopeMove.x = -SlopeRot.z * 10.0f;
	SlopeMove.z = SlopeRot.x * 10.0f;

	if (useful::CollisionCircle(GritPos, D3DXVECTOR3(Pos->x, GritPos.y, Pos->z), RAIL_WIGHT) == true)
	{// ブロックの中心にある時に上下か左右のどちらかになるまでに移動する

		Pos->x += SlopeMove.x;
		Pos->z += SlopeMove.z;
	}
	else
	{// ブロックの中心にないとき

		//上下移動
		if (GritPos.x - Pos->x >= -5.0f && GritPos.x - Pos->x <= RAIL_WIGHT)
		{
			Pos->z += SlopeMove.z;
		}
		else
		{
			Pos->z = GritPos.z;
		}

		//左右移動
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

	// 左右のグリットの番号がエラー番号(マップ外)に飛び出てる時
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

	// 上下のグリットの番号がエラー番号(マップ外)に飛び出てる時
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

	//ブロック内のリストを回してブロックとグリット番号が一致するレールの上下左右の有無を見る
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
//設置したレールと置いてあるレールの位置が同じときに移動可能方向を追加する
//====================================================================
void CRailBlock::RailAddWrite(void)
{
	//CRail* pCheckRail = m_pTop;

	////ブロック内のリストを回してブロックとグリット番号が一致するレールの上下左右の有無を見る
	//while (1)
	//{
	//	if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
	//		pRail->GetHeightNumber() == pCheckRail->GetHeightNumber())
	//	{
	//		switch (nMove[nCnt])
	//		{
	//		case 0:
	//			pCheckRail->SetRailOK(1, true);
	//			break;

	//		case 1:
	//			pCheckRail->SetRailOK(0, true);
	//			break;

	//		case 2:
	//			pCheckRail->SetRailOK(3, true);
	//			break;

	//		case 3:
	//			pCheckRail->SetRailOK(2, true);
	//			break;
	//		}
	//	}

	//	if (pRail == m_pCur)
	//	{
	//		break;
	//	}

	//	pRail = pRail->GetNextRail();
	//}
}

//====================================================================
//レールの設置処理
//====================================================================
void CRailBlock::RailSet(int Max, int* nMove)
{
	m_nMax = Max;		//レール数

	// 事前に設定したレールの設置を行う
	m_pTop = CRail::Create();
	m_pTop->SetWightNumber(GetWightNumber());
	m_pTop->SetHeightNumber(GetHeightNumber());
	m_pTop->NextSet((CRail::RAIL_POS)nMove[0]);
	m_nMove[0] = nMove[0];

	// レール設置
	CRail* pRail = m_pTop->GetNextRail();

	for (int nCnt = 1; nCnt < m_nMax; nCnt++)
	{
		m_nMove[nCnt] = nMove[nCnt];
		pRail->NextSet((CRail::RAIL_POS)nMove[nCnt]);

		//設置したレールと置いてあるレールの位置が同じときに移動可能方向を追加する
		CRail* pCheckRail = m_pTop;
		while (1)
		{
			if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
				pRail->GetHeightNumber() == pCheckRail->GetHeightNumber() &&
				pCheckRail != m_pCur)
			{
				pCheckRail->SetRailOK(nMove[nCnt], true);
			}

			pCheckRail = pCheckRail->GetNextRail();

			if (pCheckRail == nullptr)
			{
				break;
			}
		}
		//=======================

		pRail = pRail->GetNextRail();

		m_pCur = pRail;

		//設置したレールと置いてあるレールの位置が同じときに移動可能方向を追加する
		pCheckRail = m_pTop;
		while (1)
		{
			if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
				pRail->GetHeightNumber() == pCheckRail->GetHeightNumber() &&
				pCheckRail != m_pCur)
			{
				switch (nMove[nCnt])
				{
				case 0:
					pCheckRail->SetRailOK(1, true);
					break;

				case 1:
					pCheckRail->SetRailOK(0, true);
					break;

				case 2:
					pCheckRail->SetRailOK(3, true);
					break;

				case 3:
					pCheckRail->SetRailOK(2, true);
					break;
				}
			}

			if (pCheckRail == m_pCur)
			{
				break;
			}

			pCheckRail = pCheckRail->GetNextRail();
		}
		//=======================
	}
}

//====================================================================
//エディット用のレールの設置処理
//====================================================================
void CRailBlock::EditRailSet(int Number)
{
	int nWightNumber = GetWightNumber();
	int nHeightNumber = GetHeightNumber();

	if (m_pTop == nullptr)
	{
		// 事前に設定したレールの設置を行う
		m_pTop = CRail::Create();
		m_pTop->SetWightNumber(nWightNumber);
		m_pTop->SetHeightNumber(nHeightNumber);
		m_pTop->NextSet((CRail::RAIL_POS)Number);

		m_pCur = m_pTop->GetNextRail();
	}
	else
	{
		// レール設置
		CRail* pRail = m_pCur;

		nWightNumber = pRail->GetWightNumber();
		nHeightNumber = pRail->GetHeightNumber();

		pRail->SetWightNumber(nWightNumber);
		pRail->SetHeightNumber(nHeightNumber);
		pRail->NextSet((CRail::RAIL_POS)Number);

		m_pCur = pRail->GetNextRail();
	}
}

//====================================================================
//エディット用のレールの設置処理
//====================================================================
void CRailBlock::EditRailUpdate(void)
{
	CRail* pRail = m_pTop;
	int WightNumber = GetWightNumber();
	int HeightNumber = GetHeightNumber();

	if (pRail == nullptr)
	{
		return;
	}


	//ブロック内のリストを回してブロックとグリット番号が一致するレールの上下左右の有無を見る
	while (1)
	{
		if (pRail == m_pTop)
		{
			pRail->SetWightNumber(WightNumber);
			pRail->SetHeightNumber(HeightNumber);
		}
		else
		{
			switch (pRail->GetPrevRail()->GetNextNumber())
			{
			case 0:
				pRail->SetWightNumber(WightNumber);
				pRail->SetHeightNumber(HeightNumber - 1);
				break;

			case 1:
				pRail->SetWightNumber(WightNumber);
				pRail->SetHeightNumber(HeightNumber + 1);
				break;

			case 2:
				pRail->SetWightNumber(WightNumber - 1);
				pRail->SetHeightNumber(HeightNumber);
				break;

			case 3:
				pRail->SetWightNumber(WightNumber + 1);
				pRail->SetHeightNumber(HeightNumber);
				break;
			}
		}

		WightNumber = pRail->GetWightNumber();
		HeightNumber = pRail->GetHeightNumber();

		if (pRail == m_pCur)
		{
			break;
		}

		pRail = pRail->GetNextRail();
		if (pRail == nullptr)
		{
			pRail = m_pCur;
		}
	}
}

//====================================================================
//終了処理
//====================================================================
void CRailBlock::RailDelete(void)
{
	//自身が所持するレールを全て削除する
	CRail* pRail = m_pTop;
	while (1)
	{
		if (pRail != nullptr)
		{
			pRail->Uninit();

			pRail = pRail->GetNextRail();
		}
		else
		{
			break;
		}
	}
	m_pTop = nullptr;
	m_pCur = nullptr;
}

//====================================================================
//終了処理
//====================================================================
int CRailBlock::GetRailMove(int nCnt)
{
	return m_nMove[nCnt];
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRailBlock>* CRailBlock::GetList(void)
{
	return m_pList;
}
