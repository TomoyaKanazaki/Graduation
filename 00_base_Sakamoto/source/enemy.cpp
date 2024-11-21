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
#include "character.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "LevelModelEffect.h"
#include "slowManager.h"
#include "score.h"
#include "modelEffect.h"
#include "Effect.h"
#include "devil.h"
#include "DevilHole.h"
#include "objmeshField.h"
#include "sound.h"

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

	const float COORDDINATE_RATE[] = // 経路探索を行う間隔
	{
		5.0f,
		3.0f,
		1.0f
	};
	const float TARGET_DIFF = 5.0f; // 許容範囲
	const float MOVE_ASTAR = 150.0f; // 追跡時の移動速度

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
static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(EFFECT_TYPE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CEnemy>* CEnemy::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) :CCharacter(nPriority),
m_pPath(nullptr),
m_fCoordinateTimer(0.0f),
m_nTargetIndex(0),
m_nNumCoordinate(0),
m_pEffect(nullptr)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nStateCount = 0;

	m_ColorA = 1.0f;

	m_EnemyType = ENEMY_MEDAMAN;
	m_State = E_STATE_WAIT;
	m_pSlow = nullptr;
	m_SelectMove = SELECT_MOVE_MAX;

	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;

	m_Grid.x = 0;
	m_Grid.z = 0;

	m_nBugCounter = 0;
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

	switch (CManager::GetInstance()->GetScene()->GetMode())
	{
	case CScene::MODE_GAME:
		//マップとのマトリックスの掛け合わせをオンにする
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());
		break;
	case CScene::MODE_TUTORIAL:
		//マップとのマトリックスの掛け合わせをオンにする
		SetUseMultiMatrix(CTutorial::GetMapField()->GetMatrix());
		break;
	}

	// スローの生成(配属、タグの設定)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

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
	// メモリを削除
	if (m_pPath != nullptr) { delete[] m_pPath; };

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
		//m_pEffect->SetDeath();
	}

	// キャラクタークラスの終了（継承）
	CCharacter::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CEnemy::Update(void)
{
	// 値を取得
	D3DXVECTOR3 posMy = GetPos();			// 位置
	D3DXVECTOR3 posOldMy = GetPosOld();		// 前回の位置
	D3DXVECTOR3 rotMy = GetRot();			// 向き
	D3DXVECTOR3 sizeMy = GetSize();			// 大きさ

	// 過去の位置を記録
	posOldMy = posMy;

	//壁の索敵判定
	SearchWall(posMy);

	// 状態の更新
	//StateManager(posMy);

	// 移動方向処理
	Rot(rotMy);

	// 位置更新処理
	UpdatePos(posMy,posOldMy,sizeMy);

	//// プレイヤーへの最短経路探索
	//Coordinate();

	// 最短系露をたどる
	Route();

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
	CCharacter::Update();

	// デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "[敵]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);

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
	CCharacter::Draw();
}

//====================================================================
// ヒット処理
//====================================================================
bool CEnemy::Hit(int nLife)
{
	m_nLife -= nLife;

	if (m_nLife < 0)
	{// 体力0以下
		Uninit();

		// 目玉焼きを生成
		CFriedEgg::Create(m_EnemyType, m_Grid);
	}

	return true;
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
HRESULT CEnemy::InitModel(const char* pFilename)
{
	// キャラクターテキスト読み込み処理
	CCharacter::Init(pFilename);

	// マトリックス設定
	CCharacter::SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());
	CCharacter::SetUseStencil(true);

	return S_OK;
}

//====================================================================
// 位置更新処理
//====================================================================
void CEnemy::UpdatePos(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy)
{
	// モーションの取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	//重力
	m_move.y -= 0.5f;

	// 変数宣言
	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();

		if (pMotion)
		{
			pMotion->SetSlowVaule(fSpeed);
		}
	}

	CDevil* pDevil = CGame::GetDevil();

	//Y軸の位置更新
	posMy.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	posMy.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(posMy,posOldMy,sizeMy,useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X軸の位置更新
	posMy.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);
	posMy.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// 壁との当たり判定
	CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z軸の位置更新
	posMy.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);
	posMy.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

	// 壁との当たり判定
	CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);

	//ステージ外との当たり判定
	CollisionOut(posMy);
}

//====================================================================
//移動方向処理
//====================================================================
void CEnemy::Rot(D3DXVECTOR3& rotMy)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;
	fRotMove = rotMy.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	rotMy.y = atan2f(-m_move.x, -m_move.z);

	useful::NormalizeAngle(&rotMy);
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ)
{
	// キューブブロックのリスト構造が無ければ抜ける
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // リストを取得

	// キューブブロックリストの中身を確認する
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 pos = pCubeBlock->GetPos();
		D3DXVECTOR3 posOld = pCubeBlock->GetPosOld();
		D3DXVECTOR3 Move = pCubeBlock->GetMove();
		D3DXVECTOR3 Size = pCubeBlock->GetSize();
		bool bNullJump;

		// 矩形の当たり判定
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, size, &bNullJump, XYZ) == true)
		{
			//待機状態にする
			m_State = E_STATE_WAIT;

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
	//		m_State = E_STATE_WAIT;
	//	}
	//}
}

//====================================================================
// ステージ外との当たり判定
//====================================================================
void CEnemy::CollisionOut(D3DXVECTOR3& posMy)
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
		if (Pos.x + MapSize.x < posMy.x) // 右
		{
			posMy.x = Pos.x -MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > posMy.x) // 左
		{
			posMy.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < posMy.z) // 上
		{
			posMy.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > posMy.z) // 下
		{
			posMy.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//====================================================================
// 死亡処理
//====================================================================
void CEnemy::Death(void)
{
	// モーションの取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetFinish() == true)
	{
		m_ColorA -= 0.005f;

		if (m_ColorA <= 0.0f)
		{
			Uninit();
		}
	}
}

//====================================================================
// 状態更新
//====================================================================
void CEnemy::StateManager(D3DXVECTOR3& posMy)
{
	//状態の管理
	switch (m_State)
	{
	case CEnemy::E_STATE_WAIT:

		MoveSelect();
		m_State = E_STATE_WALK;

		break;

	case CEnemy::E_STATE_TRUN:

		m_move.x = m_move.x * -1.0f;
		m_move.z = m_move.z * -1.0f;

		break;

	case CEnemy::E_STATE_WALK:

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

	case CEnemy::E_STATE_EGG:
		break;

	case CEnemy::E_STATE_DEATH:

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		break;

	default:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// 移動方向の選択
//====================================================================
void CEnemy::MoveSelect()
{
	float OKRot[4];
	int RotNumber = 0;

	if (m_OKL && m_SelectMove != SELECT_MOVE_RIGHT)
	{
		OKRot[RotNumber] = D3DX_PI * -0.5f;
		RotNumber++;
	}
	if (m_OKR && m_SelectMove != SELECT_MOVE_LEFT)
	{
		OKRot[RotNumber] = D3DX_PI * 0.5f;
		RotNumber++;
	}
	if (m_OKU && m_SelectMove != SELECT_MOVE_DOWN)
	{
		OKRot[RotNumber] = D3DX_PI * 0.0f;
		RotNumber++;
	}
	if (m_OKD && m_SelectMove != SELECT_MOVE_UP)
	{
		OKRot[RotNumber] = D3DX_PI * 1.0f;
		RotNumber++;
	}

	if (RotNumber != 0)
	{
		int nRand = rand() % RotNumber;

		m_move.x = sinf(OKRot[nRand]) * 3.0f;
		m_move.z = cosf(OKRot[nRand]) * 3.0f;

		if (m_move.x >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_RIGHT;
		}
		else if (m_move.x <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_LEFT;
		}
		else if (m_move.z >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_UP;
		}
		else if (m_move.z <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_DOWN;
		}
	}
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::SearchWall(D3DXVECTOR3& posMy)
{
	bool OKR = true;	//右
	bool OKL = true;	//左
	bool OKU = true;	//上
	bool OKD = true;	//下

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_Grid.x + 1;
	int nLNumber = m_Grid.x - 1;
	int nUNumber = m_Grid.z - 1;
	int nDNumber = m_Grid.z + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_Grid.z);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_Grid.z);
	OKU = !pMapSystem->GetGritBool(m_Grid.x, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_Grid.x, nDNumber);

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (posMy.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK))
	{// グリットの中心位置に立っているなら操作を受け付ける

		if (!m_OKR && OKR)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKL && OKL)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKU && OKU)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKD && OKD)
		{
			m_State = E_STATE_WAIT;
		}

		m_OKR = OKR;	//右
		m_OKL = OKL;	//左
		m_OKU = OKU;	//上
		m_OKD = OKD;	//下
	}
	else
	{
		m_OKR = false;	//右
		m_OKL = false;	//左
		m_OKU = false;	//上
		m_OKD = false;	//下
	}
}

//==========================================
//  最短経路探索
//==========================================
void CEnemy::Coordinate()
{
	// 探索タイマーを加算
	m_fCoordinateTimer += DeltaTime::Get();

	// 探索のタイミングでない場合関数を抜ける
	if (m_fCoordinateTimer < COORDDINATE_RATE[m_EnemyType]) { return; }

	// 最短経路の次の目標をリセット
	m_nTargetIndex = 1;

	// タイマーのリセット
	m_fCoordinateTimer -= COORDDINATE_RATE[m_EnemyType];

	// 最短経路を取得
	AStar::CoordinateList Path = AStar::Generator::GetInstance()->FindPlayer({ m_Grid.x, m_Grid.z });
	m_nNumCoordinate = Path.size();

	// メモリを削除
	if (m_pPath != nullptr) { delete[] m_pPath; };

	// 最短経路に必要なグリッド数分メモリを確保
	m_pPath = new CMapSystem::GRID[m_nNumCoordinate];

	// 確保したメモリに最短経路のグリッドを格納
	for (int i = 0; i < m_nNumCoordinate; ++i)
	{
		m_pPath[i] = Path.at(i);
	}
}

//==========================================
// 最短経路をたどる
//==========================================
void CEnemy::Route()
{
	// 自身の位置・移動量取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();

	// 最短経路が無いとき
	if (m_pPath == nullptr)
	{
		return;
	}

	// 目標地点の座標を求める
	D3DXVECTOR3 path = m_pPath[m_nTargetIndex].ToWorld();

	// 次に向かうグリッドに重なったらその次の目標を設定
	if (fabsf(path.x - pos.x) <= TARGET_DIFF &&
		fabsf(path.z - pos.z) <= TARGET_DIFF) // 一定範囲内であれば
	{
		// インデックス番号を加算
		m_nTargetIndex++;
		path = m_pPath[m_nTargetIndex].ToWorld();
	}

	// 次の目標が存在しなかったら関数を抜ける
	if (m_nTargetIndex >= m_nNumCoordinate)
	{
		return;
	}

	// 次の目標位置との角度
	float RotDest = atan2f(path.z - pos.z, path.x - pos.x);

	// 次の目標位置に移動
	move = path - pos;
	D3DXVec3Normalize(&move, &move);
	move *= DeltaTime::Get() * MOVE_ASTAR;

	// 位置更新
	pos += move;

	//目的の向き
	float DiffRot = (RotDest - rot.y) * 0.1f;
	rot.y += DiffRot;

	// 位置・移動量設定
	SetPos(pos);
	SetMove(move);
	SetRot(rot);
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

//====================================================================
// リスト取得
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
