//============================================
//
//	�f�r���̏���(��) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

#include "object.h"
#include "useful.h"

//�O���錾
class CModel;
class CMotion;
class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CMoveHelpUI;
class CParameterUI;

class CEnemy;
class CBoss;
class CPlayer;

//�I�u�W�F�N�g�v���C���[�N���X
class CDevil : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CDevil(int nPriority = 2);
	~CDevil();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_BWAIT = 0,		//�퓬�ҋ@
		ACTION_SMOVE,			//�T���ړ�
		ACTION_BMOVE,			//�퓬�ړ�
		ACTION_SWAIT,			//�T���ҋ@
		ACTION_ATTACK,			//�U��
		ACTION_BDEATH,			//�퓬���̎��S
		ACTION_SDEATH,			//�T�����̎��S
		ACTION_DAMAGE,			//�_���[�W
		ACTION_MAX,				//�ő�
	};

	//�v���C���[�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_DEATH,		//���S
		STATE_WAIT,			//�ҋ@
		STATE_DAMAGE,		//�_���[�W
		STATE_MAX,			//�ő�
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CDevil* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void TutorialUpdate(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	STATE GetState(void) { return m_State; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDevilSize(D3DXVECTOR3 size) { m_DevilSize = size; }
	D3DXVECTOR3 GetDevilSize(void) { return m_DevilSize; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetModelDisp(bool Sst);

private:
	void MyObjCreate(void);		//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);		//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);	//��ԊǗ�
	void Move(void);			//�ړ�����
	void Rot(void);				//�ړ���������
	void ObjectScroll(D3DXVECTOR3 Move);	//�I�u�W�F�N�g�̃X�N���[��
	void GritScroll(D3DXVECTOR3 Move);		//�O���b�g�̃X�N���[��
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//�v���C���[���ׂ���鎞�̏���

	void DebugKey(void);		//�f�o�b�O�L�[

	void LoadLevelData(const char* pFilename);

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_size;				//�傫��
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_DevilSize;		//�f�r���p���[���y�Ԕ͈́A�ʏ̃f�r���T�C�Y�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	int m_nActionCount;				//�s���̃J�E���g
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�J�E���g

	float m_CollisionRot;			//�����蔻��p�̌���

	CSlowManager* m_pSlow;			// �X���[

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif