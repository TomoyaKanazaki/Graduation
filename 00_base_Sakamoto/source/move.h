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
    CMoveState() {};
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
    virtual void Move(CObjectCharacter* pCharacter){}                   // �ړ�����
    virtual void Move(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis) {}                   // �v���C���[�ړ�����

    CMoveState* GetMoveState(CObjectCharacter* pCharacter);             // �ړ���Ԃ̏��擾

protected:
    // �����o�֐�
    void Control(CObjectCharacter* pCharacter);             // ���쏈��
    void Control(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis);                         // �v���C���[�̑��쏈��

    void Random();              // �����_�����s
    void AStar();               // �ǐ�
    void Stop();                // ��~

private:
    // �����o�֐�
    D3DXVECTOR3 InputKey(CObjectCharacter* pCharacter, D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move, float fSpeed);		//�ړ����̓L�[�{�[�h

};

//============================================
// ����o������
//============================================
class CStateControl : public CMoveState
{
public:
    CStateControl(){}
    ~CStateControl(){}

    // �؂�ւ�����
    void ControlRandom(CObjectCharacter* pCharacter) override;      // �����_���ɐ؂�ւ�
    void ControlAStar(CObjectCharacter* pCharacter) override;       // �ǐՂɐ؂�ւ�
    void ControlStop(CObjectCharacter* pCharacter) override;        // ��~�ɐ؂�ւ�

    void Move(CObjectCharacter* pCharacter) override;      // �ړ�����
    void Move(CPlayer* pPlayer, D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis) override;                  // �v���C���[�̈ړ�����

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