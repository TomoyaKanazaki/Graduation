//============================================
//
//	移動状態の管理 [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"
#include "objectcharacter.h"
#include "manager.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#include "devil.h"
#include "DevilHole.h"

#include "camera.h"
#include "MapSystem.h"
#include "useful.h"

//==========================================
// 定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //移動量の減衰速度(卵)
	const int INVINCIBLE_TIME = 120;		//無敵時間
	const float PLAYER_SPEED = 5.0f;		//プレイヤーの移動速度
	const float ENEMY_SPEED = 3.0f;			//敵の移動速度

	const float COORDDINATE_RATE[] =		// 経路探索を行う間隔
	{
		5.0f,
		3.0f,
		1.0f
	};

	const float TARGET_DIFF = 10.0f;		// 許容範囲
	const float MOVE_ASTAR = 150.0f;		// 追跡時の移動速度

	const float GRIT_OK = 45.0f;			//移動可能なグリットの範囲内

}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//**********************************************************************************************************
// 移動状態のインターフェース
//**********************************************************************************************************
//==========================================
// コンストラクタ
//==========================================
CMoveState::CMoveState()
{
	m_State = STATE_NONE;
}

//==========================================
// 移動状態の情報取得
//==========================================
CMoveState* CMoveState::GetMoveState(CObjectCharacter* pCharacter)
{
	return pCharacter->GetMoveState();		// 現在のキャラクター状態の情報を返す
}

//==========================================
// デバッグ表示用
//==========================================
void CMoveState::Debug(void)
{
	DebugProc::Print(DebugProc::POINT_LEFT, "移動状態 : ");

	auto str = magic_enum::enum_name(m_State);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");

}

//====================================================================
// 移動方向処理
//====================================================================
void CMoveState::Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rot)
{
	//キーボードの取得
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 move = pCharacter->GetMove();

	//移動方向に向きを合わせる処理
	/*float fRotMove, fRotDest;
	fRotMove = rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;*/

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// 位置更新処理
//====================================================================
void CMoveState::UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	// 変数宣言
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 move = pCharacter->GetMove();		// 移動量
	D3DXVECTOR3 objMove = pCharacter->GetObjMove();

	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化

	//重力
	move.y -= 0.5f;
	pCharacter->SetMove(move);		// 移動量

	//Y軸の位置更新
	pos.y += move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	pos.y += objMove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// 壁との当たり判定
	//CollisionWall(pos,posOldMy,sizeMy,useful::COLLISION_Y);
	//CollisionDevilHole(useful::COLLISION_Y);

	//X軸の位置更新
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(move.x);
	pos.x += objMove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(move.x);

	// 壁との当たり判定
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_X);
	//CollisionDevilHole(useful::COLLISION_X);

	//Z軸の位置更新
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(move.z);
	pos.z += objMove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(move.z);

	// 壁との当たり判定
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_Z);
	//CollisionDevilHole(useful::COLLISION_Z);

}

//**********************************************************************************************************
// 操作できる状態
//**********************************************************************************************************
//==========================================
// コンストラクタ
//==========================================
CStateControl::CStateControl()
{
	m_bInput = false;				// 入力を行ったかどうか
	m_RotState = ROTSTATE_NONE;		// 移動方向の状態

	m_State = STATE_CONTROL;			// 操作状態

}

//==========================================
// 破棄
//==========================================
void CStateControl::Release()
{

}

//==========================================
// 操作からランダム歩行に切り替え
//==========================================
void CStateControl::ControlRandom(CObjectCharacter* pCharacter)
{
	// ランダム歩行状態にする
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// 操作から追跡に切り替え
//==========================================
void CStateControl::ControlAStar(CObjectCharacter* pCharacter)
{
	// 追跡状態にする
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// 操作から停止に切り替え
//==========================================
void CStateControl::ControlStop(CObjectCharacter* pCharacter)
{
	// 停止状態にする
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// 移動処理
//==========================================
void CStateControl::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bInput = false;			// 入力してない状態にする

	// 入力処理
	switch (pCharacter->GetType())
	{
	case CObject::TYPE_PLAYER3D:		// プレイヤー
		NormarizeMove = InputKey(pCharacter, pos, rot, NormarizeMove, PLAYER_SPEED);
		UpdateMovePlayer(pCharacter, NormarizeMove);		// 移動更新

		break;

	case CObject::TYPE_ENEMY3D:			// 敵
		NormarizeMove = InputKey(pCharacter, pos, rot, NormarizeMove, ENEMY_SPEED);
		UpdateMoveEnemy(pCharacter, NormarizeMove);			// 移動更新

		// 移動方向処理
		Rot(pCharacter, rot);

		// 位置更新処理
		UpdatePos(pCharacter, pos);

		break;

	default:
		break;
	}

	
}

//==========================================
// プレイヤーの移動更新処理
//==========================================
void CStateControl::UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove)
{
	CObjectCharacter::STATE state = pCharacter->GetState();
	D3DXVECTOR3 EggMove = pCharacter->GetEggMove();	// プレイヤーの卵移動量

	if (m_bInput && state != CObjectCharacter::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.z *= PLAYER_SPEED;

		//移動量を代入
		pCharacter->SetMove(NormarizeMove);

		if (state == CObjectCharacter::STATE_EGG)
		{
			// モデル数の取得
			int nNumModel = pCharacter->GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// モデルの取得
				CModel* pModel = pCharacter->GetModel(nCnt);

				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			EggMove.y = EGG_MOVE.y;

			switch (m_RotState)
			{
			case CMoveState::ROTSTATE_LEFT:
				EggMove.x = EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CMoveState::ROTSTATE_RIGHT:
				EggMove.x = -EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CMoveState::ROTSTATE_UP:
				EggMove.x = 0.0f;
				EggMove.z = -EGG_MOVE.z;
				break;
			case CMoveState::ROTSTATE_DOWN:
				EggMove.x = 0.0f;
				EggMove.z = EGG_MOVE.z;
				break;
			}

			// 卵の移動量設定
			pCharacter->SetEggMove(EggMove);

			//無敵状態の設定
			pCharacter->SetInvincible(true);
			pCharacter->SetInvincibleCount(INVINCIBLE_TIME);
		}
		//移動状態にする
		pCharacter->SetState(CObjectCharacter::STATE_WALK);
	}
}

//==========================================
// 敵の移動更新処理
//==========================================
void CStateControl::UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3& NormarizeMove)
{
	D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

	NormarizeMove.x *= ENEMY_SPEED;
	NormarizeMove.z *= ENEMY_SPEED;

	//移動量を代入
	pCharacter->SetMove(NormarizeMove);

	//移動状態にする
	pCharacter->SetState(CObjectCharacter::STATE_WALK);
}

//==========================================
// 移動入力キーボード(キャラクター)
//==========================================
D3DXVECTOR3 CStateControl::InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3 Move, float fSpeed)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	// 移動の進行許可状況
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();
	bool bGridCenter = pCharacter->GetGritCenter();		// グリッド座標の中心にいるか

	//キーボードの移動処理
	if ((pInputKeyboard->GetPress(DIK_W) && progress.bOKU && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_RotState == ROTSTATE_DOWN))
	{
		Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_UP;
	}
	else if (((pInputKeyboard->GetPress(DIK_S) && progress.bOKD && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_RotState == ROTSTATE_UP)) &&
		pInputKeyboard->GetPress(DIK_W) == false)
	{
		Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && progress.bOKL && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_RotState == ROTSTATE_RIGHT))
	{
		Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_LEFT;
	}
	else if (((pInputKeyboard->GetPress(DIK_D) && progress.bOKR && bGridCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_RotState == ROTSTATE_LEFT)) &&
		pInputKeyboard->GetPress(DIK_A) == false)
	{
		Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
		Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

		m_bInput = true;
		m_RotState = ROTSTATE_RIGHT;
	}

	return Move;
}

//**********************************************************************************************************
// ランダム歩行状態
//**********************************************************************************************************
//==========================================
// コンストラクタ
//==========================================
CStateRandom::CStateRandom()
{
	m_State = STATE_RANDOM;
	m_SelectGrid.x = 0;
	m_SelectGrid.z = 0;
}


//==========================================
// 破棄
//==========================================
void CStateRandom::Release()
{

}

//==========================================
// ランダム歩行から操作に切り替え
//==========================================
void CStateRandom::ControlRandom(CObjectCharacter* pCharacter)
{
	// 操作状態にする
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// ランダム歩行から追跡に切り替え
//==========================================
void CStateRandom::RandomAStar(CObjectCharacter* pCharacter)
{
	// 追跡状態にする
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// ランダム歩行から停止に切り替え
//==========================================
void CStateRandom::RandomStop(CObjectCharacter* pCharacter)
{
	// 停止状態にする
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// キャラクターの移動処理
//==========================================
void CStateRandom::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// 壁の索敵判定
	SearchWall(pCharacter, pos);

	// 移動方向の選択
	MoveSelect(pCharacter);

	// 移動方向処理
	Rot(pCharacter, rot);

	// 位置更新処理
	UpdatePos(pCharacter, pos);
}

//====================================================================
// 壁の索敵判定(移動選択の準備)
//====================================================================
void CStateRandom::SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();	// 移動の進行許可状況

	bool OKR = true;	//右
	bool OKL = true;	//左
	bool OKU = true;	//上
	bool OKD = true;	//下

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = grid.x + 1;
	int nLNumber = grid.x - 1;
	int nUNumber = grid.z - 1;
	int nDNumber = grid.z + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, grid.z);
	OKL = !pMapSystem->GetGritBool(nLNumber, grid.z);
	OKU = !pMapSystem->GetGritBool(grid.x, nUNumber);
	OKD = !pMapSystem->GetGritBool(grid.x, nDNumber);

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) &&
		(grid.x != m_SelectGrid.x || grid.z != m_SelectGrid.z))
	{// グリットの中心位置に立っているなら操作を受け付ける

		if (!progress.bOKR && OKR)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKL && OKL)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKU && OKU)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}
		if (!progress.bOKD && OKD)
		{
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
		}

		progress.bOKR = OKR;	//右
		progress.bOKL = OKL;	//左
		progress.bOKU = OKU;	//上
		progress.bOKD = OKD;	//下
	}
	else
	{
		progress.bOKR = false;	//右
		progress.bOKL = false;	//左
		progress.bOKU = false;	//上
		progress.bOKD = false;	//下
	}
}

//====================================================================
// 移動方向の選択
//====================================================================
void CStateRandom::MoveSelect(CObjectCharacter* pCharacter)
{
	float OKRot[4];
	int RotNumber = 0;
	CObjectCharacter::PROGGRESS progress = pCharacter->GetProgress();	// 移動の進行許可状況
	D3DXVECTOR3 move = pCharacter->GetMove();		// 移動量
	m_RotState = GetRotState();						// 移動方向の状態

	// 進行できる方向を確認
	if (progress.bOKL && m_RotState != CMoveState::ROTSTATE_RIGHT)
	{
		OKRot[RotNumber] = D3DX_PI * -0.5f;
		RotNumber++;
	}
	if (progress.bOKR && m_RotState != CMoveState::ROTSTATE_LEFT)
	{
		OKRot[RotNumber] = D3DX_PI * 0.5f;
		RotNumber++;
	}
	if (progress.bOKU && m_RotState != CMoveState::ROTSTATE_DOWN)
	{
		OKRot[RotNumber] = D3DX_PI * 0.0f;
		RotNumber++;
	}
	if (progress.bOKD && m_RotState != CMoveState::ROTSTATE_UP)
	{
		OKRot[RotNumber] = D3DX_PI * 1.0f;
		RotNumber++;
	}

	// 進行できる方向を決定
	if (RotNumber != 0)
	{
		int nRand = rand() % RotNumber;

		move.x = sinf(OKRot[nRand]) * 3.0f;
		move.z = cosf(OKRot[nRand]) * 3.0f;

		if (move.x >= 3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_RIGHT;
		}
		else if (move.x <= -3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_LEFT;
		}
		else if (move.z >= 3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_UP;
		}
		else if (move.z <= -3.0f)
		{
			m_RotState = CMoveState::ROTSTATE_DOWN;
		}

		m_SelectGrid = pCharacter->GetGrid();
	}

	// 移動量設定
	pCharacter->SetMove(move);
}

//**********************************************************************************************************
// 追跡状態
//**********************************************************************************************************
//==========================================
// コンストラクタ
//==========================================
CStateAStar::CStateAStar()
{
	m_pPath = nullptr;
	m_EnemyType = CEnemy::ENEMY_NONE;
	m_fCoordinateTimer = 0.0f;
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;

	m_State = STATE_ASTAR;			// 操作状態


}

//==========================================
// 初期化処理
//==========================================
void CStateAStar::Init()
{
	m_fCoordinateTimer = 0.0f;
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;
}

//==========================================
// 破棄
//==========================================
void CStateAStar::Release()
{
	// メモリを削除
	if (m_pPath != nullptr) { delete[] m_pPath; };
}

//==========================================
// 追跡から操作に切り替え
//==========================================
void CStateAStar::ControlAStar(CObjectCharacter* pCharacter)
{
	// 操作状態にする
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// 追跡からランダム歩行に切り替え
//==========================================
void CStateAStar::RandomAStar(CObjectCharacter* pCharacter)
{
	Init();

	// ランダム歩行状態にする
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// 追跡から停止に切り替え
//==========================================
void CStateAStar::AStarStop(CObjectCharacter* pCharacter)
{
	// 停止状態にする
	pCharacter->ChangeMoveState(new CStateStop);
}

//==========================================
// キャラクターの移動処理
//==========================================
void CStateAStar::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// 最短経路探索
	Coordinate(pCharacter);

	// 最短経路をたどる
	Route(pCharacter);

	// 移動方向処理
	Rot(pCharacter, rot);

	// 位置更新処理
	UpdatePos(pCharacter, pos);
}

//==========================================
//  最短経路探索
//==========================================
void CStateAStar::Coordinate(CObjectCharacter* pCharacter)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();

	// 探索タイマーを加算
	m_fCoordinateTimer += DeltaTime::Get();

	// 探索のタイミングでない場合関数を抜ける
	if (m_fCoordinateTimer < COORDDINATE_RATE[m_EnemyType]) { return; }

	// 最短経路の次の目標をリセット
	m_nTargetIndex = 1;

	// タイマーのリセット
	m_fCoordinateTimer -= COORDDINATE_RATE[m_EnemyType];

	// 最短経路を取得
	AStar::CoordinateList Path = AStar::Generator::GetInstance()->FindPlayer({ grid.x, grid.z });
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
void CStateAStar::Route(CObjectCharacter* pCharacter)
{
	// 自身の位置・移動量取得
	D3DXVECTOR3 pos = pCharacter->GetPos();
	D3DXVECTOR3 move = pCharacter->GetMove();
	D3DXVECTOR3 rot = pCharacter->GetRot();

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
	pCharacter->SetPos(pos);
	pCharacter->SetMove(move);
	pCharacter->SetRot(rot);
}

//**********************************************************************************************************
// 停止状態
//**********************************************************************************************************
//==========================================
// 破棄
//==========================================
void CStateStop::Release()
{
	
}

//==========================================
// 停止から操作に切り替え
//==========================================
void CStateStop::ControlStop(CObjectCharacter* pCharacter)
{
	// 操作状態にする
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// 停止からランダム歩行に切り替え
//==========================================
void CStateStop::RandomStop(CObjectCharacter* pCharacter)
{
	// ランダム歩行状態にする
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// 停止から追跡に切り替え
//==========================================
void CStateStop::AStarStop(CObjectCharacter* pCharacter)
{
	// 追跡状態にする
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// キャラクターの移動処理
//==========================================
void CStateStop::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{

}
