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
{}

//**********************************************************************
// ����ł�����
//**********************************************************************
//==========================================
// ���삩�烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateControl::ControlRandom(CMoveStateContext* pMoveStateContext)
{
	// �����_�����s��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// ���삩��ǐՂɐ؂�ւ�
//==========================================
void CStateControl::ControlAStar(CMoveStateContext* pMoveStateContext)
{
	// �ǐՏ�Ԃɂ���
	pMoveStateContext->ChangeState(new CStateAStar);
}

//==========================================
// ���삩���~�ɐ؂�ւ�
//==========================================
void CStateControl::ControlStop(CMoveStateContext* pMoveStateContext)
{
	// ��~��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// �����_�����s���
//**********************************************************************
//==========================================
// �����_�����s���瑀��ɐ؂�ւ�
//==========================================
void CStateRandom::ControlRandom(CMoveStateContext* pMoveStateContext)
{
	// �����Ԃɂ���
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// �����_�����s����ǐՂɐ؂�ւ�
//==========================================
void CStateRandom::RandomAStar(CMoveStateContext* pMoveStateContext)
{
	// �ǐՏ�Ԃɂ���
	pMoveStateContext->ChangeState(new CStateAStar);
}

//==========================================
// �����_�����s�����~�ɐ؂�ւ�
//==========================================
void CStateRandom::RandomStop(CMoveStateContext* pMoveStateContext)
{
	// ��~��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// �ǐՏ��
//**********************************************************************
//==========================================
// �ǐՂ��瑀��ɐ؂�ւ�
//==========================================
void CStateAStar::ControlAStar(CMoveStateContext* pMoveStateContext)
{
	// �����Ԃɂ���
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// �ǐՂ��烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateAStar::RandomAStar(CMoveStateContext* pMoveStateContext)
{
	// �����_�����s��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// �ǐՂ����~�ɐ؂�ւ�
//==========================================
void CStateAStar::AStarStop(CMoveStateContext* pMoveStateContext)
{
	// ��~��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateStop);
}

//**********************************************************************
// ��~���
//**********************************************************************
//==========================================
// ��~���瑀��ɐ؂�ւ�
//==========================================
void CStateStop::ControlStop(CMoveStateContext* pMoveStateContext)
{
	// �����Ԃɂ���
	pMoveStateContext->ChangeState(new CStateControl);
}

//==========================================
// ��~���烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateStop::RandomStop(CMoveStateContext* pMoveStateContext)
{
	// �����_�����s��Ԃɂ���
	pMoveStateContext->ChangeState(new CStateRandom);
}

//==========================================
// ��~����ǐՂɐ؂�ւ�
//==========================================
void CStateStop::AStarStop(CMoveStateContext* pMoveStateContext)
{
	// �ǐՏ�Ԃɂ���
	pMoveStateContext->ChangeState(new CStateAStar);
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