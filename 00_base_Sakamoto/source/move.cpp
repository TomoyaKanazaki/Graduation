//============================================
//
//	�ړ���Ԃ̊Ǘ� [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	
}

//==========================================
// ��ԕύX����
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