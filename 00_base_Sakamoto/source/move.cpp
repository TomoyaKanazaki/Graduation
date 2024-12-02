//============================================
//
//	�ړ���Ԃ̊Ǘ� [move.cpp]
//	Author:Satone Shion
//
//============================================
#include "move.h"
#include "objectcharacter.h"

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
void CStateControl::ControlRandom(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// ���삩��ǐՂɐ؂�ւ�
//==========================================
void CStateControl::ControlAStar(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// ���삩���~�ɐ؂�ւ�
//==========================================
void CStateControl::ControlStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//**********************************************************************
// �����_�����s���
//**********************************************************************
//==========================================
// �����_�����s���瑀��ɐ؂�ւ�
//==========================================
void CStateRandom::ControlRandom(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// �����_�����s����ǐՂɐ؂�ւ�
//==========================================
void CStateRandom::RandomAStar(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

//==========================================
// �����_�����s�����~�ɐ؂�ւ�
//==========================================
void CStateRandom::RandomStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//**********************************************************************
// �ǐՏ��
//**********************************************************************
//==========================================
// �ǐՂ��瑀��ɐ؂�ւ�
//==========================================
void CStateAStar::ControlAStar(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// �ǐՂ��烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateAStar::RandomAStar(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// �ǐՂ����~�ɐ؂�ւ�
//==========================================
void CStateAStar::AStarStop(CObjectCharacter* pCharacter)
{
	// ��~��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateStop);
}

//**********************************************************************
// ��~���
//**********************************************************************
//==========================================
// ��~���瑀��ɐ؂�ւ�
//==========================================
void CStateStop::ControlStop(CObjectCharacter* pCharacter)
{
	// �����Ԃɂ���
	pCharacter->ChangeMoveState(new CStateControl);
}

//==========================================
// ��~���烉���_�����s�ɐ؂�ւ�
//==========================================
void CStateStop::RandomStop(CObjectCharacter* pCharacter)
{
	// �����_�����s��Ԃɂ���
	pCharacter->ChangeMoveState(new CStateRandom);
}

//==========================================
// ��~����ǐՂɐ؂�ւ�
//==========================================
void CStateStop::AStarStop(CObjectCharacter* pCharacter)
{
	// �ǐՏ�Ԃɂ���
	pCharacter->ChangeMoveState(new CStateAStar);
}

#if 0
//==========================================
// ��ԕύX����
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