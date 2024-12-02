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
//class CMoveStateContext;        // �ړ���ԊǗ�
class CObjectCharacter;         // �L�����N�^�[���

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
    virtual void ControlRandom(CObjectCharacter* pCharacter) {}        // ����ƃ����_���؂�ւ�
    virtual void ControlAStar(CObjectCharacter* pCharacter) {}         // ����ƒǐՐ؂�ւ�
    virtual void ControlStop(CObjectCharacter* pCharacter) {}          // ����ƒ�~�؂�ւ�

    // �����_�����s
    virtual void RandomAStar(CObjectCharacter* pCharacter) {}           // �����_���ƒǐՐ؂�ւ�
    virtual void RandomStop(CObjectCharacter* pCharacter) {}            // �����_���ƒ�~�؂�ւ�

    // �ǐ�
    virtual void AStarStop(CObjectCharacter* pCharacter) {}            // �ǐՂƒ�~�؂�ւ�

    //virtual void Move()
};

#if 0
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

#endif

//============================================
// ����o������
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl(){}
    ~CStateControl(){}

    void ControlRandom(CObjectCharacter* pCharacter) override;      // �����_���ɐ؂�ւ�
    void ControlAStar(CObjectCharacter* pCharacter) override;       // �ǐՂɐ؂�ւ�
    void ControlStop(CObjectCharacter* pCharacter) override;        // ��~�ɐ؂�ւ�
};

//============================================
// �����_�����s���
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

    void ControlRandom(CObjectCharacter* pCharacter) override;      // ����ɐ؂�ւ�
    void RandomAStar(CObjectCharacter* pCharacter) override;        // �ǐՂɐ؂�ւ�
    void RandomStop(CObjectCharacter* pCharacter) override;         // ��~�ɐ؂�ւ�
};

//============================================
// �ǐՏ��
//============================================
class CStateAStar : public CMoveState
{
public:
    CStateAStar(){}
    ~CStateAStar() {}

    void ControlAStar(CObjectCharacter* pCharacter) override;       // ����ɐ؂�ւ�
    void RandomAStar(CObjectCharacter* pCharacter) override;        // �����_�����s�ɐ؂�ւ�
    void AStarStop(CObjectCharacter* pCharacter) override;          // ��~�ɐ؂�ւ�

};


//============================================
// ��~���
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

    void ControlStop(CObjectCharacter* pCharacter) override;        // ����ɐ؂�ւ�
    void RandomStop(CObjectCharacter* pCharacter) override;         // �����_�����s�ɐ؂�ւ�
    void AStarStop(CObjectCharacter* pCharacter) override;          // �ǐՂɐ؂�ւ�

};

#endif