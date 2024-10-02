//============================================
//
//	�v���C���[�̏��� [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

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

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,	//�ҋ@
		ACTION_MOVE,		//�ړ�
		ACTION_READY,		//�p����
		ACTION_ULTIMATE,	//�p����
		ACTION_GOATTACK,	//�ːi
		ACTION_DEATHEVENT,	//�{�X���S���C�x���g
		ACTION_MAX,			//�ő�
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
	static CPlayer* Create();
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
	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	float GetCameraHeightRot(void) { return m_HeightCameraRot; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);

	void HitDamage(float Damage);
	bool SortObject(D3DXVECTOR3 pos);					// �I�u�W�F�N�g�Ƃ̃\�[�g����

private:
	void MyObjCreate(void);		//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);		//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);	//��ԊǗ�
	void Move(void);			//�ړ�����
	void Rot(void);				//�ړ���������
	void CollisionWall(useful::COLLISION XYZ);	//�ǂƂ̓����蔻��
	void CollisionMapModel(useful::COLLISION XYZ);	//�ǂƂ̓����蔻��

	void CameraPosUpdate(void);	//�J�����ʒu�X�V����
	void PosUpdate(void);		//�ʒu�X�V����
	void RotUpdate(void);		//�����X�V����

	void DebugKey(void);		//�f�o�b�O�L�[

	void LoadLevelData(const char* pFilename);

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;	//�����ړ��̈ړ�����
	D3DXVECTOR3 m_size;			//�傫��
	float m_HeightCameraRot;	//�J�����̍����̊p�x
	D3DXVECTOR3 m_AtkPos;		//�U���ʒu
	D3DXVECTOR3 m_CameraPos;	//�J�����ʒu�ʒu
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//�s���̃J�E���g
	STATE m_State;				//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g

	float m_CollisionRot;		//�����蔻��p�̌���

	float m_fLife;					//���C�t

	CSlowManager* m_pSlow;		// �X���[

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif