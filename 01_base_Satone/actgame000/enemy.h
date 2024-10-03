//==============================================================
//
//�G�̏���[enemy.h]
//Author:����������
//
//==============================================================
#ifndef _ENEMY_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ENEMY_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"
#include "game.h"

//�}�N����`
#define ENEMY_FRAME		((MAX_ENEMY_POS * 2) + 1)		//�G�������t���[��

//�O���錾
class CModelHier;	//���f���̊K�w�\��
class CMotion;		//���[�V�����̊K�w�\��

//�G�N���X�̒�`
class CEnemy : public CObject
{
public:

	CEnemy();		//�R���X�g���N�^
	CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CEnemy();		//�f�X�g���N�^

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu�ݒ�
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }		//�����ݒ�
	void SetState(STATE state) { m_state = state; }			//��Ԑݒ�

	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//�ʒu�擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//�����擾

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }			//�傫���̍ő�l�擾
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }			//�傫���̍ŏ��l�擾

private:

	//�G�̍s�����
	enum ENEMYSTATE
	{
		ENEMYSTATE_NONE = 0,		//�������ĂȂ����
		ENEMYSTATE_MOVE,			//�����Ă���
		ENEMYSTATE_JUMP,			//�W�����v���
		ENEMYSTATE_LAND,			//���n���
		ENEMYSTATE_DASH,			//�_�b�V�����
		ENEMYSTATE_MAX
	};

	//�G�̃p�[�c
	enum PARTS
	{
		PARTS_BODY = 0,		//[0]��
		PARTS_HEAD,			//[1]��
		PARTS_HAIR,			//[2]��
		PARTS_LU_ARM,		//[3]���r��
		PARTS_LD_ARM,		//[4]���r��
		PARTS_L_HAND,		//[5]����
		PARTS_RU_ARM,		//[6]�E�r��
		PARTS_RD_ARM,		//[7]�E�r��
		PARTS_R_HAND,		//[8]�E��
		PARTS_WAIST,		//[9]��
		PARTS_LU_LEG,		//[10]��������
		PARTS_LD_LEG,		//[11]���ӂ���͂�
		PARTS_L_SHOE,		//[12]���C
		PARTS_RU_LEG,		//[13]�E������
		PARTS_RD_LEG,		//[14]�E�ӂ���͂�
		PARTS_R_SHOE,		//[15]�E�C
		PARTS_MAX
	};

	//�v���C���[�̍s���ۑ��p
	struct SaveAction
	{
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//����

		bool bMove;			//�����Ă邩�̔���
		bool bJump;			//�W�����v�������̔���
		bool bLand;			//���n������
		bool bDash;			//�_�b�V��������
		bool bDashAuto;		//�����_�b�V�����Ă邩

	};

	void UpdateFront(void);			//��O���̍X�V����
	void UpdateState(void);			//��Ԃ̍X�V����

	void MotionManager(void);				//���[�V�����Ǘ�

	void Screen(void);						//��ʊO����
	void LoadFile(void);					//���f���t�@�C���ǂݍ���
	void RotCorrection(void);				//�����̕␳����

	static char *m_apFileName[PARTS_MAX];		//�t�@�C����
	static int m_nNumAll;						//�G�̑���
	int m_nNum;									//�G�̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_max;		//�l�Ԃ̍ő�l
	D3DXVECTOR3 m_min;		//�l�Ԃ̍ŏ��l
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	CModelHier *m_apModel[PARTS_MAX];		//���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;		//���f��(�p�[�c)�̑���
	int m_nCntDamage;		//�_���[�W�J�E���^�[

	D3DXVECTOR3 m_rotSave;	//�����ۑ��p
	D3DXVECTOR3 m_moveSave;	//�ړ��ʕۑ��p

	bool m_bChaseStart;		//�ǂ������邩

	bool m_bMoveL;			//���ɕ����Ă邩�̔���
	bool m_bMoveR;			//�E�ɕ����Ă邩�̔���
	bool m_bMove;			//�����Ă邩�̔���
	bool m_bJump;			//�W�����v�������̔���
	bool m_bLand;			//���n������
	bool m_bDash;			//�_�b�V��������
	bool m_bDashAuto;		//�����_�b�V�����Ă邩

	float m_fRotDest;		//�ڕW
	float m_fRotDiff;		//����

	STATE m_state;			//�G�̏��
	ENEMYSTATE m_enemyState;		//�G�̓����̏��

	int m_nFrameCounter;	//�t���[�����J�E���^

	SaveAction m_aSaveAction[ENEMY_FRAME];		//�s���ۑ��p
	CMotion *m_pMotion;		//���[�V�������
};

#endif
