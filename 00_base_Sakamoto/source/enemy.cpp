//============================================
//
//	敵の処理 [enemy.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemy.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "objectcharacter.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "slowManager.h"
#include "score.h"
#include "devil.h"
#include "DevilHole.h"
#include "objmeshField.h"
#include "sound.h"
#include "wall.h"
#include "move.h"

#include "enemyBonbon.h"
#include "enemyMedaman.h"
#include "enemyYoungDevil.h"
#include "friedegg.h"

//===========================================
// 定数定義
//===========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 60.0f, 35.0f);		//横の当たり判定
	const float LIFE = 1.0f;	// 体力
	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内

	const float DISTANCE_RECEDE = 200.0f;	//近づく距離
	const float DISTANCE_APPROACH = 100.0f;	//遠ざかる距離

	//const float COORDDINATE_RATE[] = // 経路探索を行う間隔
	//{
	//	5.0f,
	//	3.0f,
	//	1.0f
	//};
	//const float TARGET_DIFF = 10.0f; // 許容範囲
	//const float MOVE_ASTAR = 150.0f; // 追跡時の移動速度

	const CMyEffekseer::TYPE EFFECT_TYPE[] = // 経路探索を行う間隔
	{
		CMyEffekseer::TYPE_RESPAWN_MEDAMAN,
		CMyEffekseer::TYPE_RESPAWN_BONBON,
		CMyEffekseer::TYPE_RESPAWN_BONBON // TODO : 子デビル
	};
}

//==========================================
//  静的警告処理
//==========================================
//static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(EFFECT_TYPE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CEnemy>* CEnemy::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) :CObjectCharacter(nPriority),
m_pEffect(nullptr)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nMoveStateCount = 0;

	m_ColorA = 1.0f;

	m_EnemyType = ENEMY_MEDAMAN;
	m_pSlow = nullptr;

	m_HitState = HIT_STATE_NORMAL;
	m_nHitStateCount = 0;

	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;
	m_Progress.bOKD = true;

	m_Grid.x = 0;
	m_Grid.z = 0;

	m_nBugCounter = 0;

	m_pMoveState = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemy* CEnemy::Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid)
{
	// ポインタ宣言
	CEnemy* pEnemy = nullptr;

	// タイプごとのインスタンス生成
	switch (eType)
	{
	case ENEMY_MEDAMAN: // めだまん
		pEnemy = new CEnemyMedaman;
		break;

	case ENEMY_BONBON: // ぼんぼん
		pEnemy = new CEnemyBonbon;
		break;

	case ENEMY_LITTLEDEVIL: // こでびる
		pEnemy = new CEnemyYoungDevil;
		break;

	default:
		assert(false);
		break;
	}

	//初期化処理
	if (FAILED(pEnemy->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 座標を設定
	pEnemy->SetGrid(grid);
	pEnemy->SetPos(CMapSystem::GetInstance()->GetGritPos(grid));

	// 敵のタイプを設定
	pEnemy->m_EnemyType = eType;

	// エフェクトを生成
	pEnemy->Effect();

	return pEnemy;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemy::Init(void)
{
	//種類設定
	SetType(CObject::TYPE_ENEMY3D);

	// キャラクタークラスの初期化（継承）
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// マトリックス設定
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());	// マップマトリックスと掛け合わせ
	SetUseStencil(true);	// ステンシル

	// スローの生成(配属、タグの設定)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	// 移動状態設定
	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateStop();		// 停止状態
		m_pMoveState->ControlStop(this);		// 操作できる状態
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// 移動向きの状態を設定
		m_pMoveState->SetEnemyType(m_EnemyType);					// 敵の種類を設定
	}

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemy>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEnemy::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// エフェクトを消去
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// 破棄
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// キャラクタークラスの終了（継承）
	CObjectCharacter::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CEnemy::Update(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置を記録
	posOldMy = posMy;
	SetGridOld(m_Grid);		// グリッド

	// 状態の更新
	MoveStateManager(posMy);

	// 状態の更新
	HitStateManager(posMy);

	if (m_HitState == HIT_STATE_DEATH)
	{
		return;
	}

	// 移動処理
	m_pMoveState->Move(this, posMy, rotMy);

	// Bキー
	if (pInputKeyboard->GetTrigger(DIK_B))
	{
		m_pMoveState->ControlStop(this);			// 停止 or 操作
	}
	// Nキー
	else if (pInputKeyboard->GetTrigger(DIK_N))
	{
		m_pMoveState->ControlAStar(this);			// 追跡 or 操作
		m_pMoveState->SetEnemyType(m_EnemyType);	// 敵の種類設定

	}
	// Mキー
	else if (pInputKeyboard->GetTrigger(DIK_M))
	{
		m_pMoveState->RandomAStar(this);			// ランダム or 追跡
		m_pMoveState->SetEnemyType(m_EnemyType);	// 敵の種類設定
	}

	// 自分の番号を設定
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posMy);

	//床の判定
	if (posMy.y <= 0.0f)
	{
		posMy.y = 0.0f;
		m_move.y = 0.0f;
	}

	// エフェクトを動かす
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 pos = posMy;
		pos.y += 0.5f;
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotMy, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	// デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[敵]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[敵]向き %f\n", rotMy.y);

	m_pMoveState->Debug();		// 現在の移動状態

	// 値更新
	SetPos(posMy);			// 位置
	SetPosOld(posOldMy);	// 前回の位置
	SetRot(rotMy);			// 向き
	SetSize(sizeMy);		// 大きさ
}

//====================================================================
//描画処理
//====================================================================
void CEnemy::Draw(void)
{
	// キャラクタークラスの描画（継承）
	CObjectCharacter::Draw();
}

//====================================================================
// ヒット処理
//====================================================================
bool CEnemy::Hit(void)
{
	Death();

	return true;
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CEnemy::InitModel(const char* pFilename)
{
	// キャラクターテキスト読み込み処理
	CObjectCharacter::SetTxtCharacter(pFilename);
	
	return S_OK;
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ)
{
	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // リストを取得

	// 壁リストの中身を確認する
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();
		bool bNullJump;

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, size, &bNullJump, XYZ) == true)
		{
			//待機状態にする
			SetState(STATE_WAIT);

			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// マップモデルの当たり判定
//====================================================================
void CEnemy::CollisionDevilHole(useful::COLLISION XYZ)
{
	//// デビルホールのリスト構造が無ければ抜ける
	//if (CDevilHole::GetList() == nullptr) { return; }
	//std::list<CDevilHole*> list = CDevilHole::GetList()->GetList();    // リストを取得

	//// デビルホールリストの中身を確認する
	//for (CDevilHole* pDevilHole : list)
	//{
	//	D3DXVECTOR3 pos = pDevilHole->GetPos();
	//	D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
	//	D3DXVECTOR3 Size = pDevilHole->GetSize();
	//	bool bNullJump;

	//	// 矩形の当たり判定
	//	if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
	//	{
	//		//待機状態にする
	//		m_MoveState = MOVE_STATE_WAIT;
	//	}
	//}
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CEnemy::CollisionOut(D3DXVECTOR3& posMy)
{
	// //キューブブロックのリスト構造が無ければ抜ける
	//if (CDevil::GetList() == nullptr) { return; }
	//std::list<CDevil*> list = CDevil::GetList()->GetList();    // リストを取得

	// //キューブブロックリストの中身を確認する
	//for (CDevil* pDevil : list)
	//{
	//	D3DXVECTOR3 Pos = pDevil->GetDevilPos();
	//	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	//	float GritSize = CMapSystem::GetInstance()->GetGritSize();

	//	// ステージ外の当たり判定
	//	if (Pos.x + MapSize.x < posMy.x) // 右
	//	{
	//		posMy.x = Pos.x -MapSize.x - GritSize;
	//	}
	//	if (Pos.x - MapSize.x - GritSize > posMy.x) // 左
	//	{
	//		posMy.x = Pos.x + MapSize.x;
	//	}
	//	if (Pos.z + MapSize.z + GritSize < posMy.z) // 上
	//	{
	//		posMy.z = Pos.z - MapSize.z;
	//	}
	//	if (Pos.z - MapSize.z > posMy.z) // 下
	//	{
	//		posMy.z = Pos.z + MapSize.z + GritSize;
	//	}
	//}
}

//====================================================================
// 死亡処理
//====================================================================
void CEnemy::Death(void)
{
	Uninit();

	// 目玉焼きを生成
	CFriedEgg::Create(m_EnemyType, m_Grid);

	SetState(STATE_DEATH);
}

//====================================================================
// 状態更新
//====================================================================
void CEnemy::MoveStateManager(D3DXVECTOR3& posMy)
{
	STATE state = GetState();

	//状態の管理
	switch (state)
	{
	case CObjectCharacter::STATE_WAIT:

		SetState(STATE_WALK);

		break;

	case CObjectCharacter::STATE_TRUN:

		m_move.x = m_move.x * -1.0f;
		m_move.z = m_move.z * -1.0f;

		break;

	case CObjectCharacter::STATE_WALK:

		if (abs(m_move.x) < 0.01f && abs(m_move.z) < 0.01f)
		{
			m_nBugCounter++;

			if (m_nBugCounter > 180)
			{
				posMy = m_Grid.ToWorld();
				m_nBugCounter = 0;
			}
		}

		break;

	case CObjectCharacter::STATE_DEATH:

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		break;

	default:
		break;
	}

	if (m_nMoveStateCount > 0)
	{
		m_nMoveStateCount--;
	}
}

//====================================================================
// 状態更新
//====================================================================
void CEnemy::HitStateManager(D3DXVECTOR3& posMy)
{
	//状態の管理
	switch (m_HitState)
	{
	case CEnemy::HIT_STATE_NORMAL:
		SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case CEnemy::HIT_STATE_DAMAGE:

		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_move = INITVECTOR3;

		if (m_nHitStateCount <= 0)
		{
			m_HitState = HIT_STATE_DEATH;
			Death();
		}

		break;

	case CEnemy::HIT_STATE_DEATH:

		break;

	case CEnemy::HIT_STATE_INVINCIBLE:
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case CEnemy::HIT_STATE_EGG:
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	}

	if (m_nHitStateCount > 0)
	{
		m_nHitStateCount--;
	}
}

//==========================================
//  エフェクトの生成
//==========================================
void CEnemy::Effect()
{
	// 情報を取得
	D3DXVECTOR3 pos = GetPos();
	pos.y += 0.5f;
	D3DXVECTOR3 rot = GetRot();
	D3DXMATRIX mtx = *GetUseMultiMatrix();
	pos = useful::CalcMatrix(pos, rot, mtx);
	rot = useful::CalcMatrixToRot(mtx);

	// エフェクトを生成
	m_pEffect = MyEffekseer::EffectCreate(EFFECT_TYPE[m_EnemyType], false, pos, rot, D3DXVECTOR3(10.0f, 10.0f, 10.0f));
}

//==========================================
// 移動状態変更処理
//==========================================
void CEnemy::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
}

//====================================================================
// リスト取得
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
