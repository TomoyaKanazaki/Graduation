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
#include "MapMove.h"
#include "DevilHole.h"
#include "item.h"

#include "camera.h"
#include "MapSystem.h"
#include "useful.h"

//==========================================
// 定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 // 移動量の減衰速度(卵)
	const int INVINCIBLE_TIME = 120;		// 無敵時間
	const float PLAYER_SPEED = 5.0f;		// プレイヤーの移動速度
	const float ENEMY_SPEED = 3.0f;			// 敵の移動速度
	const float FRIEDEGG_SPEED = 3.0f;		// 目玉焼きの移動速度

	const float COORDDINATE_RATE[] =		// 経路探索を行う間隔
	{
		5.0f,
		3.0f,
		1.0f
	};

	const float TARGET_DIFF = 10.0f;		// 許容範囲
	const float MOVE_ASTAR = 150.0f;		// 追跡時の移動速度

	const float GRIT_OK = 45.0f;			// 移動可能なグリットの範囲内

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

	// 進行許可情報
	m_Progress.bOKR = false;	//右
	m_Progress.bOKL = false;	//左
	m_Progress.bOKU = false;	//上
	m_Progress.bOKD = false;	//下 

	// 前回の進行許可情報
	m_ProgressOld.bOKR = false;	//右
	m_ProgressOld.bOKL = false;	//左
	m_ProgressOld.bOKU = false;	//上
	m_ProgressOld.bOKD = false;	//下 

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
// 移動方向処理(キャラクター)
//====================================================================
void CMoveState::Rot(CObjectCharacter* pCharacter, D3DXVECTOR3& rot)
{
	//キーボードの取得
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	D3DXVECTOR3 move = pCharacter->GetMove();

	//移動方向に向きを合わせる処理
	/*float fRotMove, fRotDest;
	fRotMove = rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;*/

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// 移動方向処理(アイテム)
//====================================================================
void CMoveState::Rot(CItem* pItem, D3DXVECTOR3& rot)
{
	//キーボードの取得
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	D3DXVECTOR3 move = pItem->GetMove();

	rot.y = atan2f(-move.x, -move.z);

	useful::NormalizeAngle(&rot);
}

//====================================================================
// 位置更新処理(キャラクター)
//====================================================================
void CMoveState::UpdatePos(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	// 変数宣言
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();
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
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);
	pos.x += objMove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);

	// 壁との当たり判定
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_X);
	//CollisionDevilHole(useful::COLLISION_X);

	//Z軸の位置更新
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);
	pos.z += objMove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);

	// 壁との当たり判定
	//CollisionWall(pos, posOldMy, sizeMy, useful::COLLISION_Z);
	//CollisionDevilHole(useful::COLLISION_Z);

}

//====================================================================
// 位置更新処理(アイテム)
//====================================================================
void CMoveState::UpdatePos(CItem* pItem, D3DXVECTOR3& pos)
{
	// 変数宣言
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();
	D3DXVECTOR3 move = pItem->GetMove();		// 移動量

	float fSpeed = 1.0f;	// スロー用 default1.0fで初期化

	//重力
	move.y -= 0.5f;
	pItem->SetMove(move);		// 移動量

	//位置更新
	pos.x += move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(move.x);
	pos.y += move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	pos.z += move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(move.z);
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

	// 進行許可情報
	m_Progress.bOKR = false;	//右
	m_Progress.bOKL = false;	//左
	m_Progress.bOKU = false;	//上
	m_Progress.bOKD = false;	//下 

	// 前回の進行許可情報
	m_ProgressOld.bOKR = false;	//右
	m_ProgressOld.bOKL = false;	//左
	m_ProgressOld.bOKU = false;	//上
	m_ProgressOld.bOKD = false;	//下 

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

			pCharacter->PlayerNumberDisp(true);
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
	m_RotState = ROTSTATE_NONE;

	// 進行許可情報
	m_Progress.bOKR = false;	//右
	m_Progress.bOKL = false;	//左
	m_Progress.bOKU = false;	//上
	m_Progress.bOKD = false;	//下 

	// 前回の進行許可情報
	m_ProgressOld.bOKR = false;	//右
	m_ProgressOld.bOKL = false;	//左
	m_ProgressOld.bOKU = false;	//上
	m_ProgressOld.bOKD = false;	//下 

	m_bSwitchMove = false;             // 移動方向を変えるか
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

	// 移動方向処理
	Rot(pCharacter, rot);

	// 位置更新処理
	UpdatePos(pCharacter, pos);

	DebugProc::Print(DebugProc::POINT_LEFT, "移動向き : ");

	auto str = magic_enum::enum_name(m_RotState);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");
}

//==========================================
// アイテムの移動処理
//==========================================
void CStateRandom::Move(CItem* pItem, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	// 壁の索敵判定
	SearchWall(pItem, pos);

	// 移動方向処理
	Rot(pItem, rot);

	// 位置更新処理
	UpdatePos(pItem, pos);

	DebugProc::Print(DebugProc::POINT_LEFT, "移動向き : ");

	auto str = magic_enum::enum_name(m_RotState);

	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");
}

//====================================================================
// 壁の索敵判定(キャラクター)
//====================================================================
void CStateRandom::SearchWall(CObjectCharacter* pCharacter, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pCharacter->GetGrid();

	m_ProgressOld = m_Progress;		// 現在の進行状況にする

	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // マップシステムのインスタンスを取得
	/* GRID 構造体にする*/
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// マップの横幅
	MaxGrid.z = pMapSystem->GetHeightMax(); // マップの立幅
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos(); // スクロールでずれてる幅

	/* 自身の隣接４マスのグリッド */
	int nNumber[ROTSTATE_MAX];	// 4方向の隣接するグリッド
	nNumber[ROTSTATE_LEFT] = grid.x - 1;		// 左
	nNumber[ROTSTATE_RIGHT] = grid.x + 1;		// 右
	nNumber[ROTSTATE_UP] = grid.z - 1;			// 上
	nNumber[ROTSTATE_DOWN] = grid.z + 1;		// 下

	/*
	* useful::RangeNumber(int , int , int);
	* 第三引数が第一、第二引数の範囲内に存在しない場合に丸め込みを行う関数
	* 第一引数より大きければ第一引数の値が
	* 第二引数より小さければ第二引数の値が返ってくる
	*/
	nNumber[ROTSTATE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_LEFT]);
	nNumber[ROTSTATE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_RIGHT]);
	nNumber[ROTSTATE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_UP]);
	nNumber[ROTSTATE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_DOWN]);

	// 隣接４マスが移動可能か判断する
	m_Progress.bOKL = !pMapSystem->GetGritBool(nNumber[ROTSTATE_LEFT], grid.z);
	m_Progress.bOKR = !pMapSystem->GetGritBool(nNumber[ROTSTATE_RIGHT], grid.z);
	m_Progress.bOKU = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_UP]);
	m_Progress.bOKD = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_DOWN]);

	// 前回通ったグリッドと同じ場合、通らないようにする
	if (nNumber[ROTSTATE_LEFT] == m_GridOld.x)
	{
		m_Progress.bOKL = false;
	}
	else if(nNumber[ROTSTATE_RIGHT] == m_GridOld.x)
	{
		m_Progress.bOKR = false;
	}
	else if (nNumber[ROTSTATE_UP] == m_GridOld.z)
	{
		m_Progress.bOKU = false;
	}
	else if (nNumber[ROTSTATE_DOWN] == m_GridOld.z)
	{
		m_Progress.bOKD = false;
	}

	if (grid != m_GridOld)
	{ // 前回のグリッドと位置が違う時
		if (m_Progress.bOKD != m_ProgressOld.bOKD || m_Progress.bOKL != m_ProgressOld.bOKL ||
			m_Progress.bOKR != m_ProgressOld.bOKR || m_Progress.bOKU != m_ProgressOld.bOKU)
		{ // 前回と進める方向が変わる場合

			m_bSwitchMove = true;		// 向き選択する
		}
	}

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 : %d, %d\n", grid.x, grid.z);

	// 自身の座標とグリッドの中心が許容範囲よりも小さい場合
	if (
		(pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) && // 自身の座標 <= 自身が存在しているグリッドの中心 + グリッドサイズ * 0.5f - 許容範囲
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&

		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)))
	{// グリットの中心位置に立っているなら操作を受け付ける

		if (!m_Progress.bOKR && !m_Progress.bOKL && !m_Progress.bOKU && !m_Progress.bOKD)
		{ // いずれかの進行が許可されてないとき
			// 待機状態にする
			pCharacter->SetState(CObjectCharacter::STATE_WAIT);
			//m_RotState = ROTSTATE_WAIT;
		}

		if (m_bSwitchMove == true)
		{ // 方向変えられる場合

			// 移動方向の選択
			MoveSelect(pCharacter);
			m_bSwitchMove = false;
			m_GridOld = grid;	// 前回の位置更新

		}

	}
}

//====================================================================
// 壁の索敵判定(アイテム)
//====================================================================
void CStateRandom::SearchWall(CItem* pItem, D3DXVECTOR3& pos)
{
	CMapSystem::GRID grid = pItem->GetGrid();

	m_ProgressOld = m_Progress;		// 現在の進行状況にする

	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // マップシステムのインスタンスを取得
	/* GRID 構造体にする*/
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// マップの横幅
	MaxGrid.z = pMapSystem->GetHeightMax(); // マップの立幅

	/* 自身の隣接４マスのグリッド */
	int nNumber[ROTSTATE_MAX];	// 4方向の隣接するグリッド
	nNumber[ROTSTATE_LEFT] = grid.x - 1;		// 左
	nNumber[ROTSTATE_RIGHT] = grid.x + 1;		// 右
	nNumber[ROTSTATE_UP] = grid.z - 1;			// 上
	nNumber[ROTSTATE_DOWN] = grid.z + 1;		// 下

	/*
	* useful::RangeNumber(int , int , int);
	* 第三引数が第一、第二引数の範囲内に存在しない場合に丸め込みを行う関数
	* 第一引数より大きければ第一引数の値が
	* 第二引数より小さければ第二引数の値が返ってくる
	*/
	nNumber[ROTSTATE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_LEFT]);
	nNumber[ROTSTATE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[ROTSTATE_RIGHT]);
	nNumber[ROTSTATE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_UP]);
	nNumber[ROTSTATE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[ROTSTATE_DOWN]);

	// 隣接４マスが移動可能か判断する
	m_Progress.bOKL = !pMapSystem->GetGritBool(nNumber[ROTSTATE_LEFT], grid.z);
	m_Progress.bOKR = !pMapSystem->GetGritBool(nNumber[ROTSTATE_RIGHT], grid.z);
	m_Progress.bOKU = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_UP]);
	m_Progress.bOKD = !pMapSystem->GetGritBool(grid.x, nNumber[ROTSTATE_DOWN]);

	// 前回通ったグリッドと同じ場合、通らないようにする
	if (nNumber[ROTSTATE_LEFT] == m_GridOld.x)
	{
		m_Progress.bOKL = false;
	}
	else if (nNumber[ROTSTATE_RIGHT] == m_GridOld.x)
	{
		m_Progress.bOKR = false;
	}
	else if (nNumber[ROTSTATE_UP] == m_GridOld.z)
	{
		m_Progress.bOKU = false;
	}
	else if (nNumber[ROTSTATE_DOWN] == m_GridOld.z)
	{
		m_Progress.bOKD = false;
	}

	if (grid != m_GridOld)
	{ // 前回のグリッドと位置が違う時
		if (m_Progress.bOKD != m_ProgressOld.bOKD || m_Progress.bOKL != m_ProgressOld.bOKL ||
			m_Progress.bOKR != m_ProgressOld.bOKR || m_Progress.bOKU != m_ProgressOld.bOKU)
		{ // 前回と進める方向が変わる場合

			m_bSwitchMove = true;		// 向き選択する
		}
	}

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "敵の位置 : %d, %d\n", grid.x, grid.z);

	// 自身の座標とグリッドの中心が許容範囲よりも小さい場合
	if (
		(pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) && // 自身の座標 <= 自身が存在しているグリッドの中心 + グリッドサイズ * 0.5f - 許容範囲
			pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&

			pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
			pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)))
	{// グリットの中心位置に立っているなら操作を受け付ける

		if (m_bSwitchMove == true)
		{ // 方向変えられる場合

			// 移動方向の選択
			MoveSelect(pItem);
			m_bSwitchMove = false;
			m_GridOld = grid;	// 前回の位置更新

		}

	}
}

//====================================================================
// 移動方向の選択(キャラクター)
//====================================================================
void CStateRandom::MoveSelect(CObjectCharacter* pCharacter)
{
	std::vector<D3DXVECTOR3> move = {};
	std::vector<ROTSTATE> RotState = {};

	D3DXVECTOR3 moveSave = pCharacter->GetMove();		// 移動量
	float fAngle = 0.0f;	// 向き
	
	// 各移動方向の移動量設定
	MoveAngle(moveSave, move, RotState);

	// 進行する方向を決定
	if (move.size() >= 2)
	{ // 進行できる方向が2つ以上ある場合

		int nRand = rand() % move.size();		// 進行方向決定

		m_RotState = RotState[nRand];			// 向きの状態

		// 移動量設定
		pCharacter->SetMove(move[nRand]);
	}
	else if(move.size() == 1)
	{
		m_RotState = RotState[0];			// 向きの状態

		// 移動量設定
		pCharacter->SetMove(move[0]);
	}
	else if (move.empty())
	{ // 後ろ以外進めない時
		
		// 向きの状態設定
		switch (m_RotState)
		{
		case CMoveState::ROTSTATE_LEFT:		// 左
			// 右に移動
			fAngle = D3DX_PI * 0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_RIGHT);
			break;
		case CMoveState::ROTSTATE_RIGHT:	// 右
			// 左に移動
			fAngle = D3DX_PI * -0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_LEFT);

			break;
		case CMoveState::ROTSTATE_UP:		// 上
			// 下に移動
			fAngle = D3DX_PI * 1.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_DOWN);

			break;
		case CMoveState::ROTSTATE_DOWN:		// 下
			// 上に移動
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
			RotState.push_back(ROTSTATE_UP);

			break;

		case CMoveState::ROTSTATE_WAIT:		// 待機
			// 止まる
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(0.0f, moveSave.y, 0.0f));

			break;
		default:
			break;
		}

		// 向きの状態設定
		m_RotState = RotState[0];

		// 移動量設定
		pCharacter->SetMove(move[0]);
	}

	m_ProgressOld = m_Progress;		// 現在の進行状況にする
}

//====================================================================
// 移動方向の選択(アイテム)
//====================================================================
void CStateRandom::MoveSelect(CItem* pItem)
{
	std::vector<D3DXVECTOR3> move = {};
	std::vector<ROTSTATE> RotState = {};
	D3DXVECTOR3 moveSave = pItem->GetMove();		// 移動量
	float fAngle = 0.0f;	// 向き

	// 各移動方向の移動量設定
	MoveAngle(moveSave, move, RotState);

	// 進行する方向を決定
	if (move.size() >= 2)
	{ // 進行できる方向が2つ以上ある場合

		int nRand = rand() % move.size();		// 進行方向決定

		m_RotState = RotState[nRand];			// 向きの状態

		// 移動量設定
		pItem->SetMove(move[nRand]);
	}
	else if (move.size() == 1)
	{
		m_RotState = RotState[0];			// 向きの状態

		// 移動量設定
		pItem->SetMove(move[0]);
	}
	else if (move.empty())
	{ // 後ろ以外進めない時

		// 向きの状態設定
		switch (m_RotState)
		{
		case CMoveState::ROTSTATE_LEFT:		// 左
			// 右に移動
			fAngle = D3DX_PI * 0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_RIGHT);
			break;
		case CMoveState::ROTSTATE_RIGHT:	// 右
			// 左に移動
			fAngle = D3DX_PI * -0.5f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_LEFT);

			break;
		case CMoveState::ROTSTATE_UP:		// 上
			// 下に移動
			fAngle = D3DX_PI * 1.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_DOWN);

			break;
		case CMoveState::ROTSTATE_DOWN:		// 下
			// 上に移動
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(sinf(fAngle) * FRIEDEGG_SPEED, moveSave.y, cosf(fAngle) * FRIEDEGG_SPEED));
			RotState.push_back(ROTSTATE_UP);

			break;

		case CMoveState::ROTSTATE_WAIT:		// 待機
			// 止まる
			fAngle = D3DX_PI * 0.0f;
			move.push_back(D3DXVECTOR3(0.0f, moveSave.y, 0.0f));

			break;
		default:
			break;
		}

		// 向きの状態設定
		m_RotState = RotState[0];

		// 移動量設定
		pItem->SetMove(move[0]);
	}

	m_ProgressOld = m_Progress;		// 現在の進行状況にする
}

//====================================================================
// 各方向の移動量設定
//====================================================================
void CStateRandom::MoveAngle(D3DXVECTOR3& moveSave, std::vector<D3DXVECTOR3>& move, std::vector<ROTSTATE>& rotState)
{
	float fAngle = 0.0f;	// 向き

		// 進行できる方向を確認
	if (m_Progress.bOKL/* && m_RotState != CMoveState::ROTSTATE_LEFT*/)
	{ // 左

		// 移動方向設定
		fAngle = D3DX_PI * -0.5f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_LEFT);

	}
	if (m_Progress.bOKR/* && m_RotState != CMoveState::ROTSTATE_RIGHT*/)
	{ // 右

		// 移動方向設定
		fAngle = D3DX_PI * 0.5f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_RIGHT);

	}
	if (m_Progress.bOKU/* && m_RotState != CMoveState::ROTSTATE_UP*/)
	{ // 上

		// 移動方向設定
		fAngle = D3DX_PI * 0.0f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_UP);

	}
	if (m_Progress.bOKD/* && m_RotState != CMoveState::ROTSTATE_DOWN*/)
	{ // 下

		// 移動方向設定
		fAngle = D3DX_PI * 1.0f;
		move.push_back(D3DXVECTOR3(sinf(fAngle) * ENEMY_SPEED, moveSave.y, cosf(fAngle) * ENEMY_SPEED));
		rotState.push_back(ROTSTATE_DOWN);

	}

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
	m_fCoordinateTimer = COORDDINATE_RATE[m_EnemyType];
	m_nNumCoordinate = 0;
	m_nTargetIndex = 0;

	m_State = STATE_ASTAR;			// 操作状態


}

//==========================================
// 初期化処理
//==========================================
void CStateAStar::Init()
{
	m_fCoordinateTimer = COORDDINATE_RATE[m_EnemyType];
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
		pCharacter->ChangeMoveState(new CStateRandom);		// ランダム状態にする
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
// 転がりと停止切り替え
//==========================================
void CStateStop::RollStop(CObjectX* pObjectX)
{
	// 転がり状態にする
	pObjectX->ChangeMoveState(new CStateRoll);
}

//==========================================
// キャラクターの移動処理
//==========================================
void CStateStop::Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{

}

//**********************************************************************************************************
// 転がり状態
//**********************************************************************************************************
//==========================================
// コンストラクタ
//==========================================
CStateRoll::CStateRoll()
{
	m_State = STATE_ROLL;

	// 進行許可情報
	m_Progress.bOKR = false;	//右
	m_Progress.bOKL = false;	//左
	m_Progress.bOKU = false;	//上
	m_Progress.bOKD = false;	//下 

	// 前回の進行許可情報
	m_ProgressOld.bOKR = false;	//右
	m_ProgressOld.bOKL = false;	//左
	m_ProgressOld.bOKU = false;	//上
	m_ProgressOld.bOKD = false;	//下 
}
//==========================================
// 破棄
//==========================================
void CStateRoll::Release()
{

}

//==========================================
// 転がりと停止切り替え
//==========================================
void CStateRoll::RollStop(CObjectX* pObjectX)
{
	// 停止状態にする
	pObjectX->ChangeMoveState(new CStateStop);
}

//==========================================
// 転がりの移動処理
//==========================================
void CStateRoll::Move(CObjectX* pObjectX, D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	D3DXVECTOR3 SlopeRot = CMapSystem::GetInstance()->GetMove()->GetDevilRot();		// マップの傾き
	D3DXVECTOR3 move = pObjectX->GetMove();			// 移動量
	CMapSystem::GRID grid = pObjectX->GetGrid();	// グリッド
	CObject::OBJECT_TYPE type = pObjectX->GetType();	// オブジェクトの種類

	// 傾きによる移動量設定
	move.x = -SlopeRot.z * 10.0f;
	move.z = SlopeRot.x * 10.0f;

	//自分の立っているグリットの中心位置を求める
	D3DXVECTOR3 MyGritPos = pObjectX->GetGrid().ToWorld();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();

	// 移動処理
	if (useful::CollisionCircle(MyGritPos, D3DXVECTOR3(pos.x, MyGritPos.y, pos.z), 5.0f) == true)
	{// ブロックの中心にある時に上下か左右のどちらかになるまでに移動する(傾いてない時)

		pos.x += move.x;
		pos.z += move.z;
	}
	else
	{// ブロックの中心にないとき(傾いた時)

		//上下移動
		if (MyGritPos.x - pos.x >= -5.0f && MyGritPos.x - pos.x <= 5.0f)
		{
			pos.z += move.z;
		}
		else
		{
			pos.z = MyGritPos.z;
		}

		//左右移動
		if (MyGritPos.z - pos.z >= -5.0f && MyGritPos.z - pos.z <= 5.0f)
		{
			pos.x += move.x;
		}
		else
		{
			pos.x = MyGritPos.x;
		}
	}

	// 停止しているか判定をとる
	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKR)) &&	//左
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKL)) &&	//右
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f)) &&	//上
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f)))	//下
	{// グリットの中心位置に立っているか

		int nRGridX = grid.x + 1;
		int nLGridX = grid.x - 1;

		nRGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nRGridX);
		nLGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nLGridX);

		// 左右の移動判定
		if (type == CObject::TYPE_ROLLROCK)
		{ // 岩
			SetJudg(nRGridX, grid.z, m_Progress.bOKR);
			SetJudg(nLGridX, grid.z, m_Progress.bOKL);
		}
		else if (type == CObject::TYPE_RAILBLOCK)
		{ // レールブロック
			RailCheck(CMapSystem::GRID(nRGridX, grid.z), m_Progress.bOKR);
			RailCheck(CMapSystem::GRID(nLGridX, grid.z), m_Progress.bOKL);
		}
	}

	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f)) &&	//左
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f)) &&	//右
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKD)) &&	//上
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - (GRIT_OK * m_Progress.bOKU)))	//下
	{// グリットの中心位置に立っているか

		int nUGridZ = grid.z - 1;
		int nDGridZ = grid.z + 1;

		nUGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nUGridZ);
		nDGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nDGridZ);

		// 上下の移動判定
		if (type == CObject::TYPE_ROLLROCK)
		{
			SetJudg(grid.x, nUGridZ, m_Progress.bOKU);
			SetJudg(grid.x, nDGridZ, m_Progress.bOKD);
		}
		else if (type == CObject::TYPE_RAILBLOCK)
		{ // レールブロック
			RailCheck(CMapSystem::GRID(grid.x, nUGridZ), m_Progress.bOKU);
			RailCheck(CMapSystem::GRID(grid.x, nDGridZ), m_Progress.bOKD);
		}
	}

	// 停止処理
	Stop(pos, MyGritPos, move);

	pObjectX->SetMove(move);	// 移動量設定
}

//==========================================
// レールがあるか確認
//==========================================
void CStateRoll::RailCheck(CMapSystem::GRID& grid, bool& bProgress)
{
	// レールブロックがあるか判定を取る
	if (CMapSystem::GetInstance()->GetRailGritBool(grid) == true)
	{
		bProgress = true;		// 転がることが出来る
	}
	else
	{
		bProgress = false;		// 転がることが出来ない
	}
}

//==========================================
// 転がれるか判断する処理
//==========================================
void CStateRoll::SetJudg(int& nGridPosX, int& nGridPosZ, bool& bProgress)
{
	// 転がれる場所があるか判定を取る
	if (CMapSystem::GetInstance()->GetGritBool(nGridPosX, nGridPosZ) == true)
	{
		bProgress = false;		// 転がることが出来ない
	}
	else
	{
		bProgress = true;		// 転がることが出来る
	}
}

//==========================================
// 転がらないときの位置
//==========================================
void CStateRoll::Stop(D3DXVECTOR3& pos, D3DXVECTOR3& GridPos, D3DXVECTOR3& move)
{
	// 停止している場合、その位置に固定する
	if (!m_Progress.bOKR && move.x > 0.0f)
	{
		if (pos.x > GridPos.x)
		{
			pos.x = GridPos.x;
			move.x = 0.0f;
		}
	}
	if (!m_Progress.bOKL && move.x < 0.0f)
	{
		if (pos.x < GridPos.x)
		{
			pos.x = GridPos.x;
			move.x = 0.0f;
		}
	}
	if (!m_Progress.bOKU && move.z > 0.0f)
	{
		if (pos.z > GridPos.z)
		{
			pos.z = GridPos.z;
			move.z = 0.0f;
		}
	}
	if (!m_Progress.bOKD && move.z < 0.0f)
	{
		if (pos.z < GridPos.z)
		{
			pos.z = GridPos.z;
			move.z = 0.0f;
		}
	}
}
