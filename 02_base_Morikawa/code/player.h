//========================================
//
//�v���C���[����[player.h]
//Author�F�X��x��
//
//========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

//========================================
// �O���錾
//========================================
class CModel;
class CMotion;
class CEffect;
class CGauge;
class CLockonMarker;

//========================================
//�v���C���[�N���X
//========================================
class CPlayer : public CCharacter
{
public:

	// ���
	enum PLAYERSTATE
	{// �v���C���[�̏��
		STATE_NONE = 0,
		STATE_NORMAL,		// �ʏ�
		STATE_JUMP,			// �W�����v
		STATE_ROLL,			// �]����
		STATE_MAX
	};

	enum PLAYERTYPE
	{// �v���C���[�̃^�C�v
		TYPE_NONE = 0, // �����Ȃ�
		TYPE_RABBIT, // �e
		TYPE_TURTLE, // �T
		TYPE_MAX
	};

	CPlayer(int nPriority = 4);		//�R���X�g���N�^
	~CPlayer();		//�f�X�g���N�^

	// �����o�֐�
	static CPlayer* Create(std::string pfile);

	HRESULT Init() { return S_OK; }		//�������z
	HRESULT Init(std::string pfile);
	void Uninit();
	void Update();
	void Draw();

	float GetRadius() { return m_fRadius; }		// ���a�擾
	int GetLife() { return m_nLife; }			// �̗͎擾
	int GetState() { return m_State; }			// ��Ԏ擾
	void SetDefaultType(const PLAYERTYPE type) { m_typeDefault = type; } // �f�t�H���g�^�C�v�̕ύX
	PLAYERTYPE GetDefaultType() { return m_typeDefault; } // �f�t�H���g�^�C�v�̎擾
	PLAYERTYPE GetType() { return m_type; } // �^�C�v�̎擾

	// �ÓI�����o�֐�
	static CPlayer* GetInstance() { return m_pPlayer; }

private:

	// �����o�֐�
	void DamageCounter();
	void Motion();
	void Move(D3DXVECTOR3& pos, D3DXVECTOR3& move, const float fDeltaTime);
	void Jump(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard);
	void Roll(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard, const float fDeltaTime);
	void SwitchType(CInputPad* pPad, CInputKeyboard* pKeyboard);
	void Land(D3DXVECTOR3& pos, D3DXVECTOR3& move);
	void Damage();				// �_���[�W�֐�
	void CheckStageObjRange();	// �X�e�[�W�I�u�W�F�͈̔̓`�F�b�N
	void CollisionReverseObj();	// ���]�I�u�W�F�̓����蔻��
	void Collision();	// �����蔻��
	void SpeedUp(D3DXVECTOR3& move);		// ��������

	// �����o�ϐ�
	int m_apNumModel;		// ���f��(�p�[�c)�̑���
	int m_nLife;			// �̗�
	int m_nOldMotion;		// �O��̃��[�V����
	int m_WalkCounter;		// ���s���̃J�E���^�[
	int m_State;			// ���
	int m_nDamageCount;		// �_���[�W�J�E���g
	float m_fRadius;		// ���a
	float m_fHeight;		// �g��
	float m_fRollTime;		// �]���莞��
	float m_fSpeedScale;	// �����x
	float m_fWalkSound;		// ���s��
	bool m_bJump;			// �W�����v�t���O
	bool m_bRoll;			// �]����t���O
	CEffect* m_pEffect;				// �G�t�F�N�g�̃|�C���^
	CGauge* m_pGauge;				// �Q�[�W�̃|�C���^
	CModel* m_apModel[MAX_PARTS];	// ���f���̃_�u���|�C���^
	CLockonMarker* m_pMarker;		// ���b�N�I���}�[�J�[�̃|�C���^
	PLAYERTYPE m_type;				// �^�C�v
	PLAYERTYPE m_typeDefault;		// �f�t�H���g�^�C�v

	// �ÓI�����o�ϐ�
	static CPlayer* m_pPlayer;		// ���g�̃|�C���^
};

#endif
