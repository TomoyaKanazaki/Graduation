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
#include "player.h"
#include "camera.h"

//==========================================
// 定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //移動量の減衰速度(卵)
	const int INVINCIBLE_TIME = 120;			//無敵時間
	const float PLAYER_SPEED = 5.0f;		//プレイヤーの移動速度
	const float ENEMY_SPEED = 3.0f;			//敵の移動速度

}

//**********************************************************************
// 移動状態のインターフェース
//**********************************************************************
//==========================================
// コンストラクタ
//==========================================
CMoveState::CMoveState()
{
	
}

//**********************************************************************
// 操作できる状態
//**********************************************************************
//==========================================
// コンストラクタ
//==========================================
CStateControl::CStateControl()
{
	m_bInput = false;				//入力を行ったかどうか
	m_RotState = ROTSTATE_NONE;		// 移動方向の状態
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
		break;

	default:
		break;
	}
}

//==========================================
// プレイヤーの状態更新処理
//==========================================
void CStateControl::UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove)
{
	CPlayer::STATE state = pCharacter->GetState();		// プレイヤーの状態
	D3DXVECTOR3 EggMove = pCharacter->GetEggMove();	// プレイヤーの卵移動量

	if (m_bInput && state != CPlayer::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.z *= PLAYER_SPEED;

		//移動量を代入
		pCharacter->SetMove(NormarizeMove);

		if (state == CPlayer::STATE_EGG)
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
// 敵の状態更新処理
//==========================================
void CStateControl::UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove)
{
	D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

	NormarizeMove.x *= ENEMY_SPEED;
	NormarizeMove.z *= ENEMY_SPEED;

	//移動量を代入
	pCharacter->SetMove(NormarizeMove);

	//移動状態にする
	pCharacter->SetState(CObjectCharacter::STATE_WALK);
}

//====================================================================
//移動入力キーボード(キャラクター)
//====================================================================
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

//**********************************************************************
// ランダム歩行状態
//**********************************************************************
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

//**********************************************************************
// 追跡状態
//**********************************************************************
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

//**********************************************************************
// 停止状態
//**********************************************************************
//==========================================
// 停止から操作に切り替え
//==========================================
void CStateStop::ControlStop(CObjectCharacter* pCharacter)
{
	// 操作状態にする
	pCharacter->ChangeMoveState(new CStateControl);

	////
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
// 移動状態の情報取得
//==========================================
CMoveState* CMoveState::GetMoveState(CObjectCharacter* pCharacter)
{
	return pCharacter->GetMoveState();		// 現在のキャラクター状態の情報を返す
}
