//============================================
//
//	移動状態の管理 [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"
#include "objectcharacter.h"

//==========================================
// 定数定義
//==========================================
namespace
{}

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

#if 0
//==========================================
// 状態変更処理
//==========================================
void CMoveStateContext::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
}
#endif