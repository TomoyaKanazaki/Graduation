//============================================
//
//	ˆÚ“®ó‘Ô‚ÌŠÇ— [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"

//==========================================
// ’è”’è‹`
//==========================================
namespace
{
	
}

//==========================================
// ó‘Ô•ÏXˆ—
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