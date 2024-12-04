//============================================
//
//	�L�����N�^�[�̏��� [character.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"
#include "Model.h"
#include "MapSystem.h"

class CModel;
class CMotion;
class CShadow;
class CMoveState;		// �ړ��̏��

#define MODEL_NUM		(64)	// ���f���̐�
#define FILE_NAME_SIZE	(128)	// �t�@�C�����̍ő啶����

// �L�����N�^�[�N���X
class CObjectCharacter : public CObject
{
public:
	//���
	enum STATE
	{
		STATE_WAIT = 0,		//�ҋ@
		STATE_WALK,			//����
		STATE_DEATH,		//���S
		STATE_EGG,			//��
		STATE_ATTACK,		//�U��
		STATE_TRUN,			//�����]��

		STATE_MAX,			//�ő�
		STATE_NONE
	};

	// �ړ��̐i�s�󋵂��Ǘ�����\����
	struct PROGGRESS
	{
		bool bOKL;		//���ւ̐i�s��������邩�ǂ���
		bool bOKR;		//�E�ւ̐i�s��������邩�ǂ���
		bool bOKU;		//��ւ̐i�s��������邩�ǂ���
		bool bOKD;		//���ւ̐i�s��������邩�ǂ���
	};

	CObjectCharacter(int nPriority = 3);
	~CObjectCharacter();

	static CObjectCharacter* Create(bool bShadow);

	virtual HRESULT Init(void);
	virtual HRESULT Init(int PlayNumber) { return S_OK; };

	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col);
	CModel* GetModel(int nCnt);
	CMotion* GetMotion(void);
	int GetNumModel(void) { return m_nNumModel; }

	void SetUseMultiMatrix(D3DXMATRIX* Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX* GetUseMultiMatrix(void) { return m_UseMultiMatrix; }
	void SetUseStencil(bool bUse) { m_bUseStencil = bUse; }
	void SetUseShadowMtx(bool bUse) { m_bUseShadowMtx = bUse; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetShadow(bool frag) { m_bUseShadow = frag; }
	bool GetShadow() { return m_bUseShadow; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ړ��̐ݒ�
	void SetMoveX(float moveX) { m_move.x = moveX; }	// �ړ��̐ݒ�X
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }	// �ړ��̐ݒ�Y
	D3DXVECTOR3 GetMove(void) { return m_move; }		// �ړ��̎擾
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }
	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }


	void SetTxtCharacter(const char* pFilename);

	// �ړ���ԃN���X�p
	void SetEggMove(D3DXVECTOR3 EggMove) { m_EggMove = EggMove; }	// ���̈ړ��ʐݒ�
	virtual D3DXVECTOR3 GetEggMove() { return m_EggMove; }			// ���̈ړ��ʎ擾
	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }		// ���G���ǂ���
	void SetInvincibleCount(int nInvincibleCount) { m_nInvincibleCount = nInvincibleCount; }		// ���G����

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �ړ����
	virtual void ChangeMoveState(CMoveState* pMoveState);   // �ړ���ԕύX
	CMoveState* GetMoveState() { return m_pMoveState; }	// �ړ���Ԃ̏��擾

	PROGGRESS GetProgress() { return m_Progress; }		// �ړ��̐i�s���󋵎擾
	bool GetGritCenter() { return m_bGritCenter; }		// �O���b�h�̒��S�ɂ��邩�擾

protected:

	CShadow* m_pShadow;
	PROGGRESS m_Progress;			// �ړ��̐i�s����
	CMapSystem::GRID m_Grid;		//�O���b�g�ԍ�

	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���

	bool m_bGritCenter;				//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

private:

	void LoadModel(const char* pFilename);

	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char m_aModelName[FILE_NAME_SIZE];
	int m_nNumModel;

	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu
	D3DXVECTOR3 m_size;				//�傫��

	// �}�g���b�N�X���
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX* m_UseMultiMatrix;	// �|�����킹��}�g���b�N�X

	bool m_bUseStencil;				// �X�e���V���o�b�t�@�̎g�p�̗L��
	bool m_bUseShadowMtx;			// �V���h�E�}�g���b�N�X�̎g�p�̗L��
	bool m_bUseShadow;				// �e�̎g�p�t���O

	STATE m_State;					//���
	D3DXVECTOR3 m_EggMove;			//���̓���
	bool m_bInvincible;				//���G���ǂ���
	int m_nInvincibleCount;			//���G����

	CMoveState* m_pMoveState;		// �ړ����

};

#endif
