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
#include "tutorial.h"
#include "objmeshField.h"
#include "Devil.h"
#include "debugproc.h"
#include "player.h"
#include "MapMove.h"
#include "move.h"

//==========================================
//  定数定義
//==========================================
namespace
{
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRailBlock>* CRailBlock::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRailBlock::CRailBlock(int nPriority) :CObjectX(nPriority),
m_Grid(0, 0),
m_OldGrid(0, 0),
m_SaveGrid(0, 0),
m_pMoveState(nullptr)
{
	
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CRailBlock::CRailBlock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter),
m_SaveGrid(gridCenter),
m_pMoveState(nullptr)
{
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
CRailBlock* CRailBlock::Create(CMapSystem::GRID gridCenter)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//オブジェクト3Dの生成
		pObjectBlock = new CRailBlock(3, gridCenter);
	}

	//オブジェクトの初期化処理
	if (FAILED(pObjectBlock->Init("data\\MODEL\\00_railblock.x")))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRailBlock::Init(char* pModelName)
{
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 初期化処理
	CObjectX::Init(pModelName);

	// 位置設定
	SetPos(m_Grid.ToWorld());

	// 種類の設定
	SetType(TYPE_RAILBLOCK);

	// 移動状態設定
	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateRoll();		// 転がる状態
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// 移動向きの状態を設定
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRailBlock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// スクロールをオンにする
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRailBlock::Uninit(void)
{
	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// 破棄
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PosOld = Pos;

	// グリッド情報を保存
	m_OldGrid = m_Grid;

	// 移動処理
	Move(&Pos);

	// 位置設定
	SetPos(Pos);
	SetPosOld(PosOld);

	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CRailBlock::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 親クラスの描画
	CObjectX::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 11);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 2回目の描画
	CObjectX::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//傾き時の移動処理
//====================================================================
void CRailBlock::Move(D3DXVECTOR3* Pos)
{
	// 移動処理
	m_pMoveState->Move(this, *Pos, INITVECTOR3);

	// ステージ外との当たり判定
	CollisionOut(*Pos);

	// グリッド情報を設定
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(*Pos);

	// A*判定を設定
	Coodinate();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[レールブロック] 位置 %f : %f : %f\n", Pos->x, Pos->y, Pos->z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[レールブロック] グリッド： %d : %d\n", m_Grid.x, m_Grid.z);

}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CRailBlock::CollisionOut(D3DXVECTOR3& pos)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		DebugProc::Print(DebugProc::POINT_CENTER, "%f, %f\n\n", pos.x, pos.z);

		// z奥が+

		// ステージ外の当たり判定
		if (MapSize.x + GritSize < pos.x) // 右
		{
			pos = m_OldGrid.ToWorld();
		}
		if (-MapSize.x - GritSize > pos.x) // 左
		{
			pos = m_OldGrid.ToWorld();
		}
		if (MapSize.z + GritSize < pos.z) // 上
		{
			pos = m_OldGrid.ToWorld();
		}
		if (-MapSize.z - GritSize > pos.z) // 下
		{
			pos = m_OldGrid.ToWorld();
		}
	}
}

//==========================================
//  A*ウェイトの変更処理
//==========================================
void CRailBlock::Coodinate()
{
	// 前回のグリッドと今回のグリッドが一致している場合関数を抜ける
	if (m_Grid == m_OldGrid) { return; }

	// マップ情報を取得
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// 前回のグリッドを移動可能地点に設定
	pMapSystem->SetGritBool(m_OldGrid, false);

	// 現在のグリッドを移動不可地点に設定
	pMapSystem->SetGritBool(m_Grid, true);
}

//==========================================
// 移動状態変更処理
//==========================================
void CRailBlock::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//==========================================
// リセット
//==========================================
void CRailBlock::Reset(void)
{
	// グリッドの初期化
	m_Grid = m_SaveGrid;

	// 位置設定
	SetPos(m_Grid.ToWorld());
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CRailBlock>* CRailBlock::GetList(void)
{
	return m_pList;
}
