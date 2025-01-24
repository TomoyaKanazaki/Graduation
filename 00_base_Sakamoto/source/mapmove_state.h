//============================================
//
//	�}�b�v�ړ���Ԃ̊Ǘ����� [mapmove_state.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MAPMOVE_STATE_H_
#define _MAPMOVE_STATE_H_

//============================================
// �}�b�v�ړ���Ԃ̃C���^�[�t�F�[�X�N���X
//============================================
class CMapMoveState
{
public:

	// ����
	enum ROTTYPE
	{
		ROTTYPE_UP = 0,		// ��
		ROTTYPE_DOWN,		// ��
		ROTTYPE_LEFT,		// ��
		ROTTYPE_RIGHT,		// �E
		ROTTYPE_MAX
	};

    CMapMoveState() {};
    virtual ~CMapMoveState() {}

    virtual void Init() {}         // ������
    virtual void Release() {}      // �j��
};

//============================================
// �ҋ@���
//============================================
class CMapStateWait : public CMapMoveState
{
public:
	CMapStateWait(){}
	~CMapStateWait(){}


};

//============================================
// �X�N���[�����
//============================================
class CMapStateScroll : public CMapMoveState
{
public:
	CMapStateScroll() {}
	~CMapStateScroll() {}


};

//============================================
// �X�����
//============================================
class CMapStateSlope : public CMapMoveState
{
public:
	CMapStateSlope() {}
	~CMapStateSlope() {}


};

#endif