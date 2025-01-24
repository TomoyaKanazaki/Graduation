//============================================
//
//	�}�b�v�ړ���Ԃ̊Ǘ����� [mapmove_state.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MAPMOVE_STATE_H_
#define _MAPMOVE_STATE_H_

//============================================
// �O���錾
//============================================
class CMapMove;

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

    CMapMoveState();
    virtual ~CMapMoveState() {}

    virtual void Init() {}			// ������
	virtual void Update(){}			// �X�V����

    virtual void Release() {}		// �j��
	virtual void Set(){}			// �ݒ�

	virtual void WaitScroll(CMapMove* pMapMove){}		// �ҋ@�ƃX�N���[���؂�ւ�
	virtual void WaitSlope(CMapMove* pMapMove) {}		// �ҋ@�ƌX���؂�ւ�

	// �ݒ�E�擾
	void SetRotState(ROTTYPE RotType) { m_RotType = RotType; }		// ����
	ROTTYPE GetRotState() { return m_RotType; }						// ����

protected:
	// �����o�֐�
	void SetMoveRot();		// �ړ�������ݒ�

private:

	ROTTYPE m_RotType;				// �ړ������̎��

};

//============================================
// �ҋ@���
//============================================
class CMapStateWait : public CMapMoveState
{
public:
	CMapStateWait();
	~CMapStateWait(){}

	void Init() override;		// ������
	void Update() override;		// �X�V����

};

//============================================
// �X�N���[�����
//============================================
class CMapStateScroll : public CMapMoveState
{
public:
	CMapStateScroll() {}
	~CMapStateScroll() {}

	void Update() override {}			// �X�V����
};

//============================================
// �X�����
//============================================
class CMapStateSlope : public CMapMoveState
{
public:
	CMapStateSlope();
	~CMapStateSlope() {}

	void Init() override;		// ������
	void Update() override;		// �X�V����

	void Set() override;		// �ݒ�

	void WaitSlope(CMapMove* pMapMove) override;			// �ҋ@�ƌX���؂�ւ�

private:
	bool m_bSlope;					//�X����Ԃ��ǂ���
};

#endif