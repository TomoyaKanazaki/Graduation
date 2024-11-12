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
#include "2DEffect.h"
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
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CEnemy>* CEnemy::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;

	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nStateCount = 0;

	m_ColorA = 1.0f;

	m_pCharacter = nullptr;

	m_size = COLLISION_SIZE;
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

	// 
	pEnemy->m_EnemyType = eType;

	return pEnemy;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemy::Init(void)
{
	//種類設定
	SetType(CObject::TYPE_ENEMY3D);

	//マップとのマトリックスの掛け合わせをオンにする
	SetMultiMatrix(true);

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
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemy::Update(void)
{

	// 過去の位置を記録
	m_posOld = m_pos;

	//壁の索敵判定
	SearchWall();

	// 状態の更新
	StateManager();

	// 移動方向処理
	Rot();

	// 位置更新処理
	UpdatePos();

	// 自分の番号を設定
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(m_pos);

	//床の判定
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	// キャラクターの更新
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "[敵]横 %d : 縦 %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//描画処理
//====================================================================
void CEnemy::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// キャラクターの描画
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
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
// キャラクターの取得処理
//====================================================================
CCharacter* CEnemy::GetCharacter(void)
{
	if (m_pCharacter != nullptr)
	{
		return m_pCharacter;
	}

	assert(("キャラクターの取得失敗", false));

	return nullptr;
}

//====================================================================
// モデル関連の初期化処理
//====================================================================
void CEnemy::InitModel(const char* pFilename)
{
	// キャラクターの更新
	if (m_pCharacter == nullptr)
	{
		m_pCharacter = CCharacter::Create(pFilename);
	}
}

//====================================================================
// 位置更新処理
//====================================================================
void CEnemy::UpdatePos(void)
{
	if (m_pCharacter == nullptr)
	{
		return;
	}

	// モデルの取得
	CMotion* pMotion = m_pCharacter->GetMotion();

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
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX();
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX();

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ();
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ();

	// 壁との当たり判定
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);

	//ステージ外との当たり判定
	CollisionOut();
}

//====================================================================
//移動方向処理
//====================================================================
void CEnemy::Rot(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;
	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	m_rot.y = atan2f(-m_move.x, -m_move.z);

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
// 壁との当たり判定
//====================================================================
void CEnemy::CollisionWall(useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
		{
			//待機状態にする
			m_State = E_STATE_WAIT;

			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
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
void CEnemy::CollisionOut()
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
		if (Pos.x + MapSize.x < m_pos.x) // 右
		{
			m_pos.x = Pos.x -MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > m_pos.x) // 左
		{
			m_pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < m_pos.z) // 上
		{
			m_pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > m_pos.z) // 下
		{
			m_pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//====================================================================
// 死亡処理
//====================================================================
void CEnemy::Death(void)
{
	if (m_pCharacter == nullptr)
	{
		return;
	}

	// モデルの取得
	CMotion* pMotion = m_pCharacter->GetMotion();

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
void CEnemy::StateManager()
{
	int nRand = 0;

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
				m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
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
void CEnemy::SearchWall(void)
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
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_Grid);;
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK))
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

//====================================================================
//リスト取得
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
