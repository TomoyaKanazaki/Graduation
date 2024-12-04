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
class CObjectCharacter;         // �L�����N�^�[���
class CPlayer;                  // �v���C���[���

//============================================
// �ړ���Ԃ̃C���^�[�t�F�[�X�N���X
//============================================
class CMoveState
{
public:

    // �ړ�����
    enum ROTSTATE
    {
        ROTSTATE_WAIT = 0,	// �ҋ@
        ROTSTATE_LEFT,		// ������
        ROTSTATE_RIGHT,		// �E����
        ROTSTATE_UP,		// �����
        ROTSTATE_DOWN,		// ������
        ROTSTATE_MAX,		// �ő�
        ROTSTATE_NONE
    };

    CMoveState();
    virtual ~CMoveState() {};

    // ����
    virtual void ControlRandom(CObjectCharacter* pCharacter) {}        // ����ƃ����_���؂�ւ�
    virtual void ControlAStar(CObjectCharacter* pCharacter) {}         // ����ƒǐՐ؂�ւ�
    virtual void ControlStop(CObjectCharacter* pCharacter) {}          // ����ƒ�~�؂�ւ�

    // �����_�����s
    virtual void RandomAStar(CObjectCharacter* pCharacter) {}           // �����_���ƒǐՐ؂�ւ�
    virtual void RandomStop(CObjectCharacter* pCharacter) {}            // �����_���ƒ�~�؂�ւ�

    // �ǐ�
    virtual void AStarStop(CObjectCharacter* pCharacter) {}             // �ǐՂƒ�~�؂�ւ�

    // �ړ�����
    virtual void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot){}           // �ړ�����(�L�����N�^�[)

    CMoveState* GetMoveState(CObjectCharacter* pCharacter);             // �ړ���Ԃ̏��擾

    // �ݒ�E�擾
    virtual void SetRotState(ROTSTATE RotState) {}      // �ړ������̏�Ԃ��擾
    virtual ROTSTATE GetRotState() { return ROTSTATE_NONE; }                       // �ړ������̏�Ԃ��擾

private:
    

   
};

//============================================
// ����o������
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl();
    ~CStateControl(){}

    // �؂�ւ�����
    void ControlRandom(CObjectCharacter* pCharacter) override;      // �����_���ɐ؂�ւ�
    void ControlAStar(CObjectCharacter* pCharacter) override;       // �ǐՂɐ؂�ւ�
    void ControlStop(CObjectCharacter* pCharacter) override;        // ��~�ɐ؂�ւ�

    void Move(CObjectCharacter* pCharacter, D3DXVECTOR3& pos, D3DXVECTOR3& rot) override;      // �L�����N�^�[�̈ړ�����

    // �ݒ�E�擾
    void SetRotState(ROTSTATE RotState) override { m_RotState = RotState; }      // �ړ������̏�Ԃ��擾
    ROTSTATE GetRotState() override { return m_RotState; }                       // �ړ������̏�Ԃ��擾

private:

    // �����o�֐�
    D3DXVECTOR3 InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move, float fSpeed);		//�ړ����̓L�[�{�[�h

    void UpdateMovePlayer(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove);        // �v���C���[�̈ړ��X�V����
    void UpdateMoveEnemy(CObjectCharacter* pCharacter, D3DXVECTOR3 NormarizeMove);         // �G�̈ړ��X�V����

    // �����o�ϐ�
    bool m_bInput;				//���͂��s�������ǂ���
    ROTSTATE m_RotState;        // �ړ������̏��

};

//============================================
// �����_�����s���
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

    // �؂�ւ�����
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

    // �؂�ւ�����
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

    // �؂�ւ�����
    void ControlStop(CObjectCharacter* pCharacter) override;        // ����ɐ؂�ւ�
    void RandomStop(CObjectCharacter* pCharacter) override;         // �����_�����s�ɐ؂�ւ�
    void AStarStop(CObjectCharacter* pCharacter) override;          // �ǐՂɐ؂�ւ�

};

#endif