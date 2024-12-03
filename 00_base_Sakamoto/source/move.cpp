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

}

//**********************************************************************
// 移動状態のインターフェース
//**********************************************************************
//==========================================
// 操作処理
//==========================================
void CMoveState::Control(CObjectCharacter* pCharacter)
{

}

//==========================================
// 操作処理(プレイヤー)
//==========================================
void CMoveState::Control(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	CPlayer::STATE state = pPlayer->GetState();		// プレイヤーの状態
	CPlayer::MOVE_STATE moveState = pPlayer->GetMoveState();	// プレイヤーの移動方向の状態
	D3DXVECTOR3 EggMove = pPlayer->GetEggMove();	// プレイヤーの卵移動量
	float fSpeed = pPlayer->GetSpeed();				// プレイヤーの速さ
	bool bInput = false;							// プレイヤーの入力判定

	// 入力してない状態にする
	pPlayer->SetInput(bInput);

	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//入力処理
	NormarizeMove = InputKey(pPlayer, posThis, rotThis, NormarizeMove, fSpeed);

	/*NormarizeMove = MoveInputPadStick(posThis, rotThis, NormarizeMove);

	NormarizeMove = MoveInputPadKey(posThis, rotThis, NormarizeMove);*/

	if (bInput && state != CPlayer::STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= fSpeed;
		NormarizeMove.z *= fSpeed;

		//移動量を代入
		pPlayer->GetMove() = NormarizeMove;

		if (state == CPlayer::STATE_EGG)
		{
			// モデル数の取得
			int nNumModel = pPlayer->GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// モデルの取得
				CModel* pModel = pPlayer->GetModel(nCnt);

				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			EggMove.y = EGG_MOVE.y;

			switch (moveState)
			{
			case CPlayer::MOVE_STATE_LEFT:
				EggMove.x = EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_RIGHT:
				EggMove.x = -EGG_MOVE.x;
				EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_UP:
				EggMove.x = 0.0f;
				EggMove.z = -EGG_MOVE.z;
				break;
			case CPlayer::MOVE_STATE_DOWN:
				EggMove.x = 0.0f;
				EggMove.z = EGG_MOVE.z;
				break;
			}

			// 卵の移動量設定
			pPlayer->SetEggMove(EggMove);

			//無敵状態の設定
			pPlayer->SetInvincible(true);
			pPlayer->SetInvincibleCount(INVINCIBLE_TIME);
		}
		//移動状態にする
		pPlayer->SetState(CPlayer::STATE_WALK);
	}
}

//====================================================================
//移動入力キーボード
//====================================================================
D3DXVECTOR3 CMoveState::InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis, D3DXVECTOR3 Move, float fSpeed)
{
	////キーボードの取得
	//CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	////キーボードの移動処理
	//if ((pInputKeyboard->GetPress(DIK_W) && m_OKU && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_W) && m_MoveState == CPlayer::MOVE_STATE_DOWN))
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_UP;
	//}
	//else if (((pInputKeyboard->GetPress(DIK_S) && m_OKD && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP)) &&
	//	pInputKeyboard->GetPress(DIK_W) == false)
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_DOWN;
	//}
	//else if ((pInputKeyboard->GetPress(DIK_A) && m_OKL && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_LEFT;
	//}
	//else if (((pInputKeyboard->GetPress(DIK_D) && m_OKR && m_bGritCenter) ||
	//	(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT)) &&
	//	pInputKeyboard->GetPress(DIK_A) == false)
	//{
	//	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
	//	D3DXMATRIX mat = *GetUseMultiMatrix();
	//	D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
	//	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

	//	Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * fSpeed;
	//	Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * fSpeed;

	//	m_bInput = true;
	//	m_MoveState = MOVE_STATE_RIGHT;
	//}

	return Move;
}

//**********************************************************************
// 操作できる状態
//**********************************************************************
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
void CStateControl::Move(CObjectCharacter* pCharacter)
{
	// 操作できる処理
	Control(pCharacter);
}

//==========================================
// 移動処理(プレイヤー)
//==========================================
void CStateControl::Move(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	// 操作できる処理
	Control(pPlayer, posThis, rotThis);
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
