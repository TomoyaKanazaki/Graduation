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
#include "MapMove.h"

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

	const CMyEffekseer::TYPE EFFECT_TYPE[] = // 経路探索を行う間隔
	{
		CMyEffekseer::TYPE_RESPAWN_MEDAMAN,
		CMyEffekseer::TYPE_RESPAWN_BONBON,
		CMyEffekseer::TYPE_RESPAWN_SMALLDEVIL
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
	pEnemy->m_Grid = grid;
	pEnemy->m_SaveGrid = grid;
	//pEnemy->SetGrid(pEnemy->m_Grid);
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetGritPos(grid);
	InitPos.y = 0.0f;
	pEnemy->SetPos(InitPos);

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

	// スローの生成(配属、タグの設定)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	// 移動状態設定
	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateRandom();		// ランダム状態
		//m_pMoveState->ControlStop(this);		// 操作できる状態
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

	// スクロールをオンにする
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEnemy::Uninit(void)
{
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

	if (m_pSlow != nullptr)
	{
		m_pSlow = nullptr;
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
	D3DXVECTOR3 posThis = GetPos();			// 位置
	D3DXVECTOR3 posOldThis = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotThis = GetRot();			// 向き
	D3DXVECTOR3 sizeThis = GetSize();			// 大きさ

	// 過去の位置を記録
	posOldThis = posThis;
	//SetGridOld(m_Grid);		// グリッド
	m_GridOld = m_Grid;			// 現在のグリッド位置

	// 状態の更新
	MoveStateManager(posThis);

	// 状態の更新
	HitStateManager(posThis);

	if (m_HitState == HIT_STATE_DEATH)
	{
		return;
	}

	// 移動処理
	m_pMoveState->Move(this, posThis, rotThis);

	// Bキー
	if (pInputKeyboard->GetTrigger(DIK_B))
	{
		m_pMoveState->ControlStop(this);			// 停止 or 操作
	}
	// Nキー
	else if (pInputKeyboard->GetTrigger(DIK_N))
	{
		m_pMoveState->ControlAStar(this);			// 追跡 or 操作

	}
	// Mキー
	else if (pInputKeyboard->GetTrigger(DIK_M))
	{
		m_pMoveState->RandomAStar(this);			// ランダム or 追跡
	}

	// 自分の番号を設定
	m_Grid = CMapSystem::GetInstance()->CalcGrid(posThis);

	//床の判定
	if (posThis.y <= 0.0f)
	{
		posThis.y = 0.0f;
		m_move.y = 0.0f;
	}

	// エフェクトを動かす
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 pos = posThis;
		pos.y += 0.5f;
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotThis, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// キャラクタークラスの更新（継承）
	CObjectCharacter::Update();

	m_pMoveState->Debug();		// 現在の移動状態

	// 壁のリスト構造が無ければ抜ける
	if (CWall::GetList() == nullptr) { assert(false); }
	std::list<CWall*> listWall = CWall::GetList()->GetList();    // リストを取得

	// 壁リストの中身を確認する
	for (CWall* pWall : listWall)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, nullptr, useful::COLLISION_X) ||
			useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, nullptr, useful::COLLISION_Z)
			)
		{
			//待機状態にする
			SetState(STATE_WALK);
			// 向き状態の設定
			//m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
			m_pMoveState->RandomAStar(this);
			break;
		}
	}

	// デビルホールのリスト構造が無ければ抜ける
	if (CDevilHole::GetList() == nullptr) { return; }
	std::list<CDevilHole*> listDevil = CDevilHole::GetList()->GetList();    // リストを取得

	// デビルホールリストの中身を確認する
	for (CDevilHole* pDevilHole : listDevil)
	{
		D3DXVECTOR3 pos = pDevilHole->GetPos();
		D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
		D3DXVECTOR3 Size = pDevilHole->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, pos, pos - posOld, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, nullptr, useful::COLLISION_X) ||
			useful::CollisionBlock(pos, pos, pos - posOld, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, nullptr, useful::COLLISION_Z)
			)
		{
			//待機状態にする
			SetState(STATE_WALK);
			// 向き状態の設定
			//m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
			m_pMoveState->RandomAStar(this);
			break;
		}
	}

	// 値更新
	SetPos(posThis);			// 位置
	SetPosOld(posOldThis);		// 前回の位置
	SetRot(rotThis);			// 向き
	SetSize(sizeThis);			// 大きさ


#ifdef _DEBUG
#if 0
	if (CMapSystem::GetInstance()->GetGritBool(m_Grid))
	{
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_TRUE, false, useful::CalcMatrix(posThis, INITVECTOR3, *GetUseMultiMatrix()), INITVECTOR3, { 10.0f, 10.0f, 10.0f });
	}
#endif
#endif
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
// リセット
//====================================================================
void CEnemy::Reset(void)
{
	// グリッドの位置初期化
	m_Grid = m_SaveGrid;

	// 位置設定
	SetPos(m_Grid.ToWorld());
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CEnemy::InitModel(const std::string pFilename)
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

		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

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
	m_pMoveState->SetEnemyType(m_EnemyType);	// 敵の種類設定
	m_pMoveState->Init();
}

//====================================================================
// リスト取得
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
