//============================================
//
//	�ړ���Ԃ̊Ǘ����� [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

////�ړ��N���X
//class CMove
//{
//public:
//
//	CMove();
//	~CMove();
//
//	static CMove* Create();
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	// �ÓI�����o�֐�
//	static CListManager<CMove>* GetList(void); // ���X�g�擾
//
//private:
//
//	// �ÓI�����o�ϐ�
//	static CListManager<CMove>* m_pList; // �I�u�W�F�N�g���X�g
//
//	// �����o�ϐ�
//	CListManager<CMove>::AIterator m_iterator; // �C�e���[�^�[
//
//};

//============================================
// �ړ���Ԃ̃C���^�[�t�F�[�X�N���X
//============================================
class CMoveState
{
public:
    CMoveState() {};
    virtual ~CMoveState() {};

    virtual void ChangeWalk() = 0;      // ���s���@�؂�ւ�(�����_��or�ǐ�)
    virtual void Stop() = 0;
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


};

//============================================
// �����_�����s���
//============================================
class CStateRandom : public CMoveState
{
public:
    CStateRandom(){}
    ~CStateRandom() {}

};

//============================================
// �ǐՏ��
//============================================
class CStateChase : public CMoveState
{
public:
    CStateChase(){}
    ~CStateChase() {}

};


//============================================
// ��~���
//============================================
class CStateStop : public CMoveState
{
public:
    CStateStop(){}
    ~CStateStop() {}

};

#endif