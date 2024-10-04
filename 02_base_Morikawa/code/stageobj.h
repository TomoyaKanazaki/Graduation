//=============================================================================
// 
//  �X�e�[�W�̔z�u���w�b�_�[ [stageobj.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STAGEOBJ_H_
#define _STAGEOBJ_H_		// ��d�C���N���[�h�h�~

#include "object.h"
#include "listmanager.h"
#include "player.h"

// �O���錾
class CModel;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�W�̔z�u���N���X
class CStageObj : public CObject
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum State
	{
		STATE_NONE = 0,		// ����
		STATE_APPEARANCE,	// �o��
		STATE_LEAVE,		// �ޏ�
		STATE_MAX
	};

	// ���
	enum Type
	{
		TYPE_BG = 0,	// �w�i
		TYPE_OBSTACLE,	// ��Q��
		TYPE_REVERSE,	// ���]
		TYPE_MAX
	};

	CStageObj(int nPriority = 3);
	~CStageObj();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void SetPos(D3DXVECTOR3 pos) override;
	virtual void Kill();		// �폜
	void SetState(const State& state);	// ��Ԑݒ�
	void CollisionRange(const MyLib::Vector3& rPos);	// �͈͔���
	virtual bool Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state);	// �v���C���[�Ƃ̓����蔻��

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@param	type	[in]	���
	@param	pos		[in]	�ʒu
	*/
	static CStageObj* CreateBg(const MyLib::Vector3& pos);
	static CStageObj* CreateObstacle(const MyLib::Vector3& pos, const int type);
	static CStageObj* CreateReverse(const MyLib::Vector3& pos);
	static CListManager<CStageObj> GetList() { return m_List; }	// ���X�g�擾

protected:

	// �����o�ϐ�
	CModel* m_pModel;		// ���f���|�C���^

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CStageObj::*SAMPLE_FUNC)();
	static SAMPLE_FUNC m_SampleFuncList[];	// ��Ԋ֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateAppearance();	// �o��
	void StateLeave();		// �ޏ�

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���
	Type m_type;			// ���
	bool m_bWorking;		// �ғ�����
	static CListManager<CStageObj> m_List;	// ���X�g

};


#endif