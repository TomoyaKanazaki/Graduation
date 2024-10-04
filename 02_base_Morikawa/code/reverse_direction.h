//=============================================================================
// 
//  ���]���o�w�b�_�[ [reverse_direction.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _REVERSE_DIRECTION_H_
#define _REVERSE_DIRECTION_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���]���o�N���X
class CReverse_Direction : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_APPEARANCE,	// �o��
		STATE_WAIT,			// �ҋ@
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CReverse_Direction(int nPriority = 7);
	~CReverse_Direction();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(const State& state);	// ��Ԑݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CReverse_Direction* Create();

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CReverse_Direction::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateAppearance();	// �o��
	void StateWait();		// �ҋ@
	void StateFadeout();	// �t�F�[�h�A�E�g

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�

};


#endif