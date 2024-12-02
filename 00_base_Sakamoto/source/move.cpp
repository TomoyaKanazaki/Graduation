//============================================
//
//	移動状態の管理 [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"

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
void CStateControl::ControlRandom(CMoveStateContext* pMoveStateContext)
{
	// ランダム歩行状態にする
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// 操作から追跡に切り替え
//==========================================
void CStateControl::ControlAStar(CMoveStateContext* pMoveStateContext)
{
	// 追跡状態にする
	pMoveStateContext->ChangeState(new CStateAStar);
}

//==========================================
// 操作から停止に切り替え
//==========================================
void CStateControl::ControlStop(CMoveStateContext* pMoveStateContext)
{
	// 停止状態にする
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// ランダム歩行状態
//**********************************************************************
//==========================================
// ランダム歩行から操作に切り替え
//==========================================
void CStateRandom::ControlRandom(CMoveStateContext* pMoveStateContext)
{
	// 操作状態にする
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// ランダム歩行から追跡に切り替え
//==========================================
void CStateRandom::RandomAStar(CMoveStateContext* pMoveStateContext)
{
	// 追跡状態にする
	pMoveStateContext->ChangeState(new CStateAStar);
}

//==========================================
// ランダム歩行から停止に切り替え
//==========================================
void CStateRandom::RandomStop(CMoveStateContext* pMoveStateContext)
{
	// 停止状態にする
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// 追跡状態
//**********************************************************************
//==========================================
// 追跡から操作に切り替え
//==========================================
void CStateAStar::ControlAStar(CMoveStateContext* pMoveStateContext)
{
	// 操作状態にする
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// 追跡からランダム歩行に切り替え
//==========================================
void CStateAStar::RandomAStar(CMoveStateContext* pMoveStateContext)
{
	// ランダム歩行状態にする
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// 追跡から停止に切り替え
//==========================================
void CStateAStar::AStarStop(CMoveStateContext* pMoveStateContext)
{
	// 停止状態にする
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// 停止状態
//**********************************************************************
//==========================================
// 停止から操作に切り替え
//==========================================
void CStateStop::ControlStop(CMoveStateContext* pMoveStateContext)
{
	// 操作状態にする
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// 停止からランダム歩行に切り替え
//==========================================
void CStateStop::RandomStop(CMoveStateContext* pMoveStateContext)
{
	// ランダム歩行状態にする
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// 停止から追跡に切り替え
//==========================================
void CStateStop::AStarStop(CMoveStateContext* pMoveStateContext)
{
	// 追跡状態にする
	pMoveStateContext->ChangeState(new CStateAStar);
}

//==========================================
// 状態変更処理
//==========================================
void CMoveStateContext::ChangeState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
}