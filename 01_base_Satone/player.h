//==============================================================
//
//�v���C���[�̃��f������[player.h]
//Author:����������
//
//==============================================================
#ifndef _PLAYER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�O���錾
class CModelHier;	//���f���̊K�w�\��
class CMotion;		//���[�V�����̊K�w�\��
class CModel;
class CObject3D;

//�v���C���[�N���X�̒�`
class CPlayer : public CObject
{
public:

	//�Z�[�u�|�C���g
	enum POINT
	{
		POINT_DEFAULT = 0,		//�`���[�g���A���̏����l
		POINT_ARTBRIDGE,		//�`���[�g���A����1�Ԗ�
		POINT_ENEMYBRIDGE,		//�`���[�g���A����2�Ԗ�

		POINT_START,			//�Q�[���̏����l
		POINT_DOWN_FLOOR,		//�Q�[����1�Ԗ�
		POINT_PIER,				//�Q�[����2�Ԗ�
		POINT_MAZE,				//�Q�[����3�Ԗ�
		POINT_BIGINEND,			//�Q�[����4�Ԗ�
		POINT_FALLING,			//�Q�[����5�Ԗ�
		POINT_MAX
	};

	CPlayer();		//�R���X�g���N�^
	CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CPlayer();		//�f�X�g���N�^

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(void);

	//�ݒ菈��
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu�ݒ�
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }		//�����ݒ�
	void SetState(STATE state) { m_state = state; }			//��Ԑݒ�

	void SetDash(int nCntDash) { m_nDashCounter = nCntDash; }	//�_�b�V���񐔐ݒ�
	void SetDashAuto(bool bDashAuto);		//�����_�b�V���̐ݒ�

	//�擾����
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//�ʒu�擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//�����擾
	D3DXVECTOR3 GetMove(void) { return m_move; }			//�ړ��ʎ擾

	int GetPressFrame(void) { return m_nPressCounter; }		//�L�[���������t���[����
	int GetNumSavePoint(void) { return m_nNumPosSave; }		//���Ԗڂ̃Z�[�u�|�C���g���擾

	bool GetIsJump(void) { return m_bJump; }				//�W�����v�̎擾
	bool GetIsMove(void) { return m_bMove; }				//�ړ��̎擾
	bool GetIsDash(void) { return m_bDash; }				//�_�b�V���̎擾
	bool GetIsLand(void) { return m_bLand; }				//���n�̎擾
	bool GetIsDashAuto(void) { return m_bDashAuto; }		//�����_�b�V���̔���擾

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }			//�傫���̍ő�l�擾
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }			//�傫���̍ŏ��l�擾

private:
	//�v���C���[�̃p�[�c
	enum PARTS
	{
		PARTS_BODY = 0,		//[0]��
		PARTS_MAX
	};

	void UpdateFront(void);			//��O���̍X�V����
	void UpdateState(void);			//��Ԃ̍X�V����

	void MotionManager(void);				//���[�V�����Ǘ�
	void ControlFrontKeyboard(void);		//�v���C���[�L�[�{�[�h����(��O��)
	void ControlFrontKeyboardMove(void);	//�v���C���[�L�[�{�[�h�̈ړ�����(��O��)
	void ControlFrontKeyboardJump(void);	//�v���C���[�L�[�{�[�h�̃W�����v����(��O��)
	void ControlFrontKeyboardDash(void);	//�v���C���[�L�[�{�[�h�̃_�b�V������(��O��)

	void ControlFrontJoyPad(void);			//�v���C���[�p�b�h����(��O��)
	void ControlFrontJoyPadMove(void);		//�v���C���[�p�b�h�̈ړ�����(��O��)
	void ControlFrontJoyPadJump(void);		//�v���C���[�p�b�h�̃W�����v����(��O��)
	void ControlFrontJoyPadDash(void);		//�v���C���[�p�b�h�̃_�b�V������(��O��)

	void Screen(void);						//��ʊO����
	void SavePoint(void);					//�Z�[�u�|�C���g����
	void LoadFile(void);					//���f���t�@�C���ǂݍ���
	void RotCorrection(void);				//�����̕␳����
	void SEStep(void);				//�����炷����
	void CollisionAction(void);		//�����������̏���

	static char *m_apFileName[PARTS_MAX];	//�t�@�C����

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_posSavePoint[POINT_MAX];	//�S�Ă̕����p�̈ʒu�ۑ��p
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_moveSave;	//�ړ��ʕۑ��p
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_max;		//�l�Ԃ̍ő�l
	D3DXVECTOR3 m_min;		//�l�Ԃ̍ŏ��l
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	CModelHier *m_apModel[PARTS_MAX];		//���f��(�p�[�c)�ւ̃|�C���^
	CModel* m_pModel[2];		// ���f��
	CObject3D* m_apObject3D[2];	// �v���C���[


	int m_nNumModel;		//���f��(�p�[�c)�̑���
	int m_nCntDamage;		//�_���[�W�J�E���^�[

	int m_nPressCounter;	//�L�[���������t���[����

	bool m_bMove;			//�����Ă邩�̔���
	bool m_bJump;			//�W�����v�������̔���
	bool m_bLand;			//���n������
	bool m_bDash;			//�_�b�V��������
	bool m_bDashAuto;		//�����_�b�V�����邩
	bool m_bCollisionAlpha;		//�����u���b�N�ɓ���������

	float m_fRotDest;		//�ڕW
	float m_fRotDiff;		//����

	STATE m_state;			//�v���C���[�̏��

	int m_nDashCounter;		//�_�b�V��������

	int m_particleType;		//�p�[�e�B�N���̎��
	int m_nType;			//���Ԗڂ̃p�[�e�B�N����
	int m_nParticleLife;	//�p�[�e�B�N���̎���
	int m_nCntHit;			//�q�b�g�ł���܂ł̃J�E���^�[
	int m_nCntMove;			//�����̃J�E���^�[
	int m_nCntSand;			//���̃p�[�e�B�N���J�E���^�[

	int m_nNumPosSave;		//���Ԗڂ̃Z�[�u�|�C���g��

	bool m_bPad;			//�p�b�h�̃X�e�B�b�N��|���Ă邩

	//CMotion *m_pMotion;		//���[�V�������
};

#endif
