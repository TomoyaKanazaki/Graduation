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
#include "MapMove.h"

// �O���錾
class CModel;
class CMotion;
class CShadow;
class CMoveState;		// �ړ��̏��

#define MODEL_NUM		(64)	// ���f���̐�

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

private:

	// ��{���\����
	struct SInfo
	{
		D3DXVECTOR3 pos;			//�ʒu
		D3DXVECTOR3 posOld;			//�ߋ��̈ʒu
		D3DXVECTOR3 rot;			//����
		D3DXVECTOR3 size;			//�傫��

		// �R���X�g���N�^
		SInfo() : pos(INITVECTOR3), posOld(INITVECTOR3), rot(INITVECTOR3), size(INITVECTOR3) {}
	};

	// �g�p�K�w�\�����f���Ǘ��p�̍\����
	struct SBodyData
	{
		CModel** ppModel;		//���f���ւ̃|�C���^
		int nNumModel;			//���f���̑���

		// �R���X�g���N�^
		SBodyData() : ppModel(nullptr), nNumModel(0) {}
	};

public:


	CObjectCharacter(int nPriority = 3);	// �R���X�g���N�^
	virtual ~CObjectCharacter();			// �f�X�g���N�^

	// ����
	static CObjectCharacter* Create(bool bShadow);

	// �����o�֐�
	virtual HRESULT Init(void);	// �ėp������
	virtual HRESULT Init(int PlayNumber) { return S_OK; }	// TODO ����Ӗ��킩���
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void PlayerNumberDisp(bool Set) {}
	void SetTxtCharacter(const std::string pFilename);
	void SetModelColor(CModel::COLORTYPE Type, const D3DXCOLOR& Col);

	void SetModel(CModel* pModel, int nCnt);
	CModel* GetModel(int nCnt);
	CModel** GetModel(void);
	void SetMotion(CMotion* pMotion);
	CMotion* GetMotion(void);

	void SetNumModel(int nNumModel);
	int GetNumModel(void) { return m_BodyInfo.nNumModel; }
	void SetUseMultiMatrix(D3DXMATRIX* Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX* GetUseMultiMatrix(void) { return m_UseMultiMatrix; }
	void SetUseShadowMtx(bool bUse) { m_bUseShadowMtx = bUse; }

	// �Q�b�g�Z�b�g
	void SetPos(D3DXVECTOR3 pos) { m_Info.pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_Info.pos; }

	void SetRot(D3DXVECTOR3 rot) { m_Info.rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_Info.rot; }

	void SetPosOld(D3DXVECTOR3 posOld) { m_Info.posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_Info.posOld; }

	void SetSize(D3DXVECTOR3 size) { m_Info.size = size; }
	D3DXVECTOR3 GetSize(void) { return m_Info.size; }

	void SetShadow(bool frag) { m_bUseShadow = frag; }
	bool GetShadow() { return m_bUseShadow; }

	void SetMove(D3DXVECTOR3 move) { m_move = move; }	// �ړ��̐ݒ�
	D3DXVECTOR3 GetMove(void) { return m_move; }		// �ړ��̎擾
	void SetMoveX(float moveX) { m_move.x = moveX; }	// �ړ��̐ݒ�X
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }	// �ړ��̐ݒ�Y

	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }

	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }

	void SetOldState(STATE State) { m_OldState = State; }
	STATE GetOldState(void) { return m_OldState; }

	// �ړ���ԃN���X�p
	void SetEggMove(D3DXVECTOR3 EggMove) { m_EggMove = EggMove; }	// ���̈ړ��ʐݒ�
	virtual D3DXVECTOR3 GetEggMove() { return m_EggMove; }			// ���̈ړ��ʎ擾
	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }		// ���G���ǂ���
	bool GetInvincible(void) { return m_bInvincible; }		// ���G���ǂ���

	void SetInvincibleCount(int nInvincibleCount) { m_nInvincibleCount = nInvincibleCount; }		// ���G����
	int GetInvincibleCount(void) { return m_nInvincibleCount; }		// ���G����

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }
	virtual void SetGridOld(const CMapSystem::GRID& posOld) { m_GridOld = posOld; }
	CMapSystem::GRID GetGridOld(void) { return m_GridOld; }

	virtual void SetPlayNumber(int Number) { ; }
	virtual int GetPlayNumber(void) { assert(false); return -1; }	//�����͎g�p����Ȃ�

	// �ړ����
	virtual void ChangeMoveState(CMoveState* pMoveState);   // �ړ���ԕύX
	CMoveState* GetMoveState() { return m_pMoveState; }	// �ړ���Ԃ̏��擾

	void SetProgress(PROGGRESS progress) { m_Progress = progress; }		// �ړ��̐i�s���󋵐ݒ�
	PROGGRESS GetProgress() { return m_Progress; }		// �ړ��̐i�s���󋵎擾
	bool GetGritCenter() { return m_bGritCenter; }		// �O���b�h�̒��S�ɂ��邩�擾

	void SetSpeedState(CMapMove::SPEED State) { m_SpeedState = State; }	//���x
	CMapMove::SPEED GetSpeedState(void) { return m_SpeedState; }	//���x
	CMapMove::SPEED GetOldSpeedState(void) { return m_OldSpeedState; }	//���x

protected:

	CShadow* m_pShadow;
	PROGGRESS m_Progress;			// �ړ��̐i�s����
	CMapSystem::GRID m_Grid;		//�O���b�g�ԍ�
	CMapSystem::GRID m_GridOld;		// �O��̃O���b�g�ԍ�
	CMapSystem::GRID m_SaveGrid;	// �O���b�g�ԍ�(�ۑ��p)

	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���

	bool m_bGritCenter;				//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

private:

	void SpeedStateManager(void);
	void LoadModel(const std::string pFilename);

	CMotion* m_pMotion;
	int m_nCharacterNum;

	SInfo m_Info;
	SBodyData m_BodyInfo;	// �K�w���f�����
	CMapMove::SPEED m_SpeedState;	//���x
	CMapMove::SPEED m_OldSpeedState;	//���x

	// �}�g���b�N�X���
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX* m_UseMultiMatrix;	// �|�����킹��}�g���b�N�X


	bool m_bUseShadowMtx;			// �V���h�E�}�g���b�N�X�̎g�p�̗L��
	bool m_bUseShadow;				// �e�̎g�p�t���O

	STATE m_State;					//���
	STATE m_OldState;				// �O��̏��
	D3DXVECTOR3 m_EggMove;			//���̓���
	bool m_bInvincible;				//���G���ǂ���
	int m_nInvincibleCount;			//���G����

	CMoveState* m_pMoveState;		// �ړ����
};

#endif