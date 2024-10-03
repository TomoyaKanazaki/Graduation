//==============================================================
//
//�`���[�g���A������[tutorial.h]
//Author:����������
//
//==============================================================
#ifndef _TUTORIAL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "manager.h"

//�}�N����`
#define NUM_TUTORIAL_TEX		(3)		//�e�N�X�`���̐�

//�O���錾
class CScore;			//�X�R�A�̏��
class CPlayerModel;		//�v���C���[�̏��
class CEnemyModel;		//�G�̏��
class CModel;			//���f���̏��
class CWall;			//�ǂ̏��
class CField;			//���̏��
class CEdit;			//�G�f�B�b�g�̏��
class CBulletRemain;	//�e��
class CObject2D;		//�I�u�W�F�N�g2D

//�`���[�g���A����ʃN���X�̒�`
class CTutorial : public CScene
{
public:

	//�X�e�b�v
	enum STEP
	{
		STEP_SYNOP_FIRST = 0,		//���炷��1
		STEP_SYNOP_SECOND,			//���炷��2
		STEP_SYNOP_THIRD,			//���炷��3
		STEP_SYNOP_FORTH,			//���炷��4
		STEP_MOVE,					//�ړ����@
		STEP_JUMP,					//�W�����v���@
		STEP_BULLET,				//������
		STEP_RULE,					//���[��
		STEP_MAX
	};

	CTutorial();
	~CTutorial();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Step(void);		//�X�e�b�v����

	static CPlayerModel *GetPlayerModel(void) { return m_pPlayerModel; }				//�v���C���[�̏��
	static CBulletRemain *GetBulletRemain(void) { return m_pBulletRemain; }				//�c�e���̏��
	static int GetStep(void) { return m_step; }
	D3DXVECTOR3 GetMove(void) { return m_move; }			//�ړ��ʂ̎擾

private:
	static CScore *m_pScore;		//�X�R�A�̏��
	static CPlayerModel *m_pPlayerModel;	//�v���C���[�̏��
	static CEnemyModel *m_pEnemyModel;		//�G�̏��
	static CModel *m_pModel;		//���f���̏��
	static CWall *m_pWall;			//�ǂ̏��
	static CField *m_pField;		//���̏��
	static CEdit *m_pEdit;			//�G�f�B�b�g�̏��
	static CBulletRemain *m_pBulletRemain;		//�c�e���̏��

	static bool bReset;				//���Z�b�g������

	static char *m_apFileName[STEP_MAX];		//�t�@�C����

	static int m_step;		//���݂̃X�e�b�v

	static CObject2D *m_pObject2D[NUM_TUTORIAL_TEX];		//�I�u�W�F�N�g2D�̏��

	int m_nIdxTexture[NUM_TUTORIAL_TEX];			//�e�N�X�`���ԍ�
	D3DXVECTOR3 m_move;			//�ړ���
	bool m_bMoveL;				//���Ɉړ�����
	bool m_bMoveR;				//�E�Ɉړ�����
	bool m_bPad;				//�p�b�h�̃X�e�B�b�N��|������
	int m_nNumBullet;			//��������

};

#endif

