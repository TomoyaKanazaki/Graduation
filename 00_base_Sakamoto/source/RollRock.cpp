//============================================
//
//	転がる岩の処理 [RollRock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RollRock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"
#include "MapSystem.h"
#include "objmeshField.h"
#include "wall.h"
#include "MapMove.h"
#include "move.h"
#include "shadow.h"
#include "player.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		//当たり判定
	const float GRIT_OK = 45.0f;				//移動可能なグリットの範囲内
	const float SHADOW_SIZE = 100.0f;			// 丸影の大きさ
}

//====================================================================
//静的メンバ変数宣言
//====================================================================
CListManager<CRollRock>* CRollRock::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CRollRock::CRollRock(int nPriority) : CObjectX(nPriority),
m_pEffect(nullptr),
m_pMoveState(nullptr),
m_pShadow(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_Grid.x = 0;
	m_Grid.z = 0;
}

//====================================================================
//コンストラクタ(オーバーロード)
//====================================================================
CRollRock::CRollRock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter),
m_SaveGrid(gridCenter),
m_pEffect(nullptr),
m_pMoveState(nullptr),
m_pShadow(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CRollRock::~CRollRock()
{

}

//====================================================================
//生成処理
//====================================================================
CRollRock* CRollRock::Create(CMapSystem::GRID gridCenter)
{
	CRollRock* pSample = nullptr;

	if (pSample == nullptr)
	{
		//オブジェクト2Dの生成
		pSample = new CRollRock(3, gridCenter);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init("data\\MODEL\\00_rock.x")))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRollRock::Init(char* pModelName)
{
	m_pEffect = nullptr;

	// 位置
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	pos.y = 50.0f;

	SetType(CObject::TYPE_ROLLROCK);

	CObjectX::Init(pModelName);

	if (m_pShadow == nullptr)
	{// 影生成
		m_pShadow = CShadow::Create(pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE));
	}

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// 位置設定
	CObjectX::SetPos(pos);

	// 移動量設定
	CObjectX::SetMove(INITVECTOR3);

	SetSize(SAMPLE_SIZE);

	// 移動状態設定
	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateRoll();		// 転がる状態
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// 移動向きの状態を設定
	}

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CRollRock>::Create();
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
void CRollRock::Uninit(void)
{
	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// 破棄
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// 影の終了
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
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

	// エフェクトを消去
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CRollRock::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posThis = GetPos();			// 位置
	D3DXVECTOR3 posOldThis = GetPosOld();	// 前回の位置
	D3DXVECTOR3 rotThis = GetRot();			// 向き
	D3DXVECTOR3 sizeThis = GetSize();		// 大きさ

	//更新前の位置を過去の位置とする
	posOldThis = posThis;
	m_OldGrid = m_Grid;
	CObjectX::SetPosOld(posThis);

	Move(posThis, rotThis);

	// 移動した分だけ回転
	rotThis.z -= (posThis.x - posOldThis.x) * D3DX_PI * 0.01f;

	// 移動した分だけ回転
	rotThis.x += (posThis.z - posOldThis.z) * D3DX_PI * 0.01f;

	// ステージ外との当たり判定
	CollisionOut(posThis);

	// グリッド情報を設定
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posThis);

	// プレイヤーとの当たり判定
	CollisionPlayer();

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	if (m_pShadow != nullptr)
	{// シャドウの更新
		m_pShadow->SetPos(D3DXVECTOR3(posThis.x, 1.0f, posThis.z));
		//m_pShadow->SetBaseHeight(pos.y);
	}

	// 値更新
	SetPos(posThis);		// 位置
	SetPosOld(posOldThis);	// 前回の位置
	SetRot(rotThis);		// 向き
	SetSize(sizeThis);		// 大きさ

	//頂点情報の更新
	CObjectX::Update();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[岩]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//描画処理
//====================================================================
void CRollRock::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
// リセット
//====================================================================
void CRollRock::Reset(void)
{
	// グリッドの位置を初期化
	m_Grid = m_SaveGrid;

	// 位置
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	pos.y = 50.0f;

	// 位置設定
	CObjectX::SetPos(pos);

	// 移動量設定
	CObjectX::SetMove(INITVECTOR3);
}

//====================================================================
//傾き時の移動処理
//====================================================================
void CRollRock::Move(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// 移動処理
	m_pMoveState->Move(this, pos, rot);

	D3DXVECTOR3 move = GetMove();	// 移動量

	// エフェクトの生成
	if (m_pEffect == nullptr && (fabsf(move.x) > 0.1f || fabsf(move.z) > 0.1f))
	{
		D3DXVECTOR3 rotEf = INITVECTOR3;
		rotEf.y = atan2f(-move.x, -move.z);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotEf, mat);
		m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ROLL, true, ef, rotEf);
	}

	// エフェクトを移動
	if (m_pEffect != nullptr)
	{
		// 計算に使用する値の取得
		D3DXMATRIX mat = *GetUseMultiMatrix();

		// 座標と向きにマトリックスを反映
		D3DXVECTOR3 posEf = useful::CalcMatrix(pos, rot, mat);

		// エフェクトに情報を適用
		m_pEffect->SetPosition(posEf);
	}

	// エフェクトを消去
	if (m_pEffect != nullptr && fabsf(move.x) <= 0.1f && fabsf(move.z) <= 0.1f)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CRollRock::CollisionOut(D3DXVECTOR3& pos)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 Pos = pDevil->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize();

		// ステージ外の当たり判定
		if (Pos.x + MapSize.x < pos.x) // 右
		{
			pos.x = Pos.x - MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > pos.x) // 左
		{
			pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < pos.z) // 上
		{
			pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > pos.z) // 下
		{
			pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//==========================================
//  プレイヤーとの当たり判定
//==========================================
void CRollRock::CollisionPlayer()
{
	// 前回のグリッドと今回のグリッドが一致している場合関数を抜ける
	if (m_Grid == m_OldGrid) { return; }

	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	// プレイヤーリストの中身を確認する
	for (CPlayer* player : list)
	{
		// 死んでる場合は取得できない
		if (player->GetState() == CPlayer::STATE_EGG || player->GetState() == CPlayer::STATE_DEATH)
		{
			continue;
		}

		// プレイヤーの座標(グリッド単位)を取得
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// 存在座標が一致していた場合殺す
		if (m_Grid == gridPlayer)
		{
			player->Death();
		}
	}

	// 経路探索用の情報を取得
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// マップ情報を取得
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// 前回のグリッドを移動可能地点に設定
	generator->removeCollision(m_OldGrid.ToAStar());
	pMapSystem->SetGritBool(m_OldGrid, false);

	// 現在のグリッドを移動不可地点に設定
	generator->addCollision(m_Grid.ToAStar());
	pMapSystem->SetGritBool(m_Grid, true);
}

//==========================================
// 移動状態変更処理
//==========================================
void CRollRock::ChangeMoveState(CMoveState* pMoveState)
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

//====================================================================
//リスト取得
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}