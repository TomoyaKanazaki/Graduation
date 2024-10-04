//========================================
//
//�L�����N�^�[�S�ʏ���[character.h]
//Author�F�X��x��
//
//========================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "motion.h"

//========================================
//�L�����N�^�[�N���X
//========================================
class CCharacter : public CObject
{
public:
	enum STATE
	{// �L�����N�^�[�̏��
		STATE_NONE = 0,
		STATE_NORMAL,		// �ʏ�
		STATE_DAMAGE,		// �_���[�W
		STATE_DEATH,		// ���S
		STATE_MAX
	};

	CCharacter(int nPriority = 4);		//�R���X�g���N�^
	~CCharacter();		//�f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) { return S_OK; }		//�������z
	HRESULT Init(std::string pfile);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

	int GetMotionType() { return m_pMotion->GetType(); }				// ���[�V�����̎�ގ擾
	bool GetMotionLoop(int nType) { return m_pMotion->IsLoop(nType); }	// ���[�V�����̃��[�v�擾

	CMotion* GetMotion() { return m_pMotion; }	// ���[�V�������擾

	// �ړI�̌����̐ݒ�Ǝ擾
	void SetRotDest(D3DXVECTOR3 RotDest) { m_RotDest = RotDest; }
	D3DXVECTOR3 GetRotDest() { return m_RotDest; }

	// ��Ԏ擾
	int GetState() { return m_nState; }

private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`��
	int m_nIdxTexture;				// �e�N�X�`���̔ԍ�
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	LPD3DXMESH m_pMesh;				// ���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;		// �}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;				// �}�e���A���̐�

	D3DXVECTOR3 m_vtxMin, m_vtxMax;	// ���f���̍ő�A�ŏ��l

	CModel* m_apModel[MAX_PARTS];	// ���f���ւ̃|�C���^
	CMotion* m_pMotion;		// ���[�V�����̃|�C���^
	D3DXVECTOR3 m_RotDest;	// �ړI�̌���

	int m_apNumModel;		// ���f��(�p�[�c)�̑���
	int m_nOldMotion;		// �O��̃��[�V����
	int m_nState;			// ���݂̏��

	bool m_bJump;			//�W�����v
	bool m_bMove;			//�ړ�
	bool m_bWait;			//�ҋ@
	bool m_bMowingdown;		// �U��
	bool m_bCutdown;		// �؂艺�낵
	bool m_bStrongAttack;	// ���U��
};

#endif
