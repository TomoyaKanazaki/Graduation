//============================================
//
//	�ړ���Ԃ̊Ǘ����� [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

//============================================
// �O���錾
//============================================
class CMoveStateContext;        // �ړ���ԊǗ�

//============================================
// �ړ���Ԃ̃C���^�[�t�F�[�X�N���X
//============================================
class CMoveState
{
public:
    CMoveState() {};
    virtual ~CMoveState() {};

    // ���

    // ����
    virtual void ControlRandom(CMoveStateContext* pMoveStateContext) {}        // ����ƃ����_���؂�ւ�
    virtual void ControlAStar(CMoveStateContext* pMoveStateContext) {}         // ����ƒǐՐ؂�ւ�
    virtual void ControlStop(CMoveStateContext* pMoveStateContext) {}          // ����ƒ�~�؂�ւ�

    // �����_�����s
    virtual void RandomAStar(CMoveStateContext* pMoveStateContext) {}           // �����_���ƒǐՐ؂�ւ�
    virtual void RandomStop(CMoveStateContext* pMoveStateContext) {}            // �����_���ƒ�~�؂�ւ�

    // �ǐ�
    virtual void AStarStop(CMoveStateContext* pMoveStateContext) {}            // �ǐՂƒ�~�؂�ւ�

};

//============================================
// �ړ���ԊǗ��N���X
//============================================
class CMoveStateContext
{
public:

    CMoveStateContext() { m_pMoveState = nullptr; }
    ~CMoveStateContext() {}

    void ChangeState(CMoveState* pMoveState);   // ��ԕύX

private:

    CMoveState* m_pMoveState;       // �ړ���Ԃ̏��

};

//============================================
// ����o������
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl(){}
    ~CStateControl(){}

    void ControlRandom(CMoveStateContext* pMoveStateContext) override;      // �����_���ɐ؂�ւ�
    void ControlAStar(CMoveStateContext* pMoveStateContext) override;       // �ǐՂɐ؂�ւ�
    void ControlStop(CMoveStateContext* pMoveStateContext) override;        // ��~�ɐ؂�ւ�
};

//============================================
// �����_�����s���
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

    void ControlRandom(CMoveStateContext* pMoveStateContext) override;      // ����ɐ؂�ւ�
    void RandomAStar(CMoveStateContext* pMoveStateContext) override;        // �ǐՂɐ؂�ւ�
    void RandomStop(CMoveStateContext* pMoveStateContext) override;         // ��~�ɐ؂�ւ�
};

//============================================
// �ǐՏ��
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar(){}
    ~CStateAStar() {}

    void ControlAStar(CMoveStateContext* pMoveStateContext) override;       // ����ɐ؂�ւ�
    void RandomAStar(CMoveStateContext* pMoveStateContext) override;        // �����_�����s�ɐ؂�ւ�
    void AStarStop(CMoveStateContext* pMoveStateContext) override;          // ��~�ɐ؂�ւ�

};


//============================================
// ��~���
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

    void ControlStop(CMoveStateContext* pMoveStateContext) override;        // ����ɐ؂�ւ�
    void RandomStop(CMoveStateContext* pMoveStateContext) override;         // �����_�����s�ɐ؂�ւ�
    void AStarStop(CMoveStateContext* pMoveStateContext) override;          // �ǐՂɐ؂�ւ�

};

#endif