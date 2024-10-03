//==============================================================
//
//���[�V��������[motion.h]
//Author:����������
//
//==============================================================
#ifndef _MOTION_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MOTION_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_PARTS		(18)		//�p�[�c�ő吔
#define MAX_KEY			(32)		//�L�[�ő吔

//�O���錾
class CModelHier;			//���f���̊K�w�\��

//���[�V�����N���X�̒�`
class CMotion
{
public:
	//���[�V�����e�L�X�g
	enum MOTIONTEXT
	{
		MOTIONTEXT_PLAYER = 0,		//�v���C���[
		MOTIONTEXT_MAX
	};

	//���[�V�����̎��
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		//�ҋ@
		MOTIONTYPE_MOVE,			//�ړ�
		MOTIONTYPE_ACTION,			//�A�N�V����
		MOTIONTYPE_JUMP,			//�W�����v
		MOTIONTYPE_LAND,			//���n

		MOTIONTYPE_MAX
	};

	//�L�[�̍\����
	struct KEY
	{
		float fPosX;		//�ʒuX
		float fPosY;		//�ʒuY
		float fPosZ;		//�ʒuZ
		float fRotX;		//����X
		float fRotY;		//����Y
		float fRotZ;		//����Z

	};

	//�L�[���̍\����
	struct KEY_INFO
	{
		int nFrame;		//�Đ��t���[��
		KEY aKey[MAX_PARTS];	//�e���f���̃L�[�v�f
	};

	//���[�V�������̍\����
	struct INFO
	{
		bool bLoop;		//���[�v���邩�ǂ���
		int nNumKey;	//�L�[�̑���
		KEY_INFO aKeInfo[MAX_KEY];		//�L�[���

	};

	CMotion();		//�R���X�g���N�^
	CMotion(MOTIONTEXT textType);		//�R���X�g���N�^
	~CMotion();		//�f�X�g���N�^

	static CMotion *Create(MOTIONTEXT textType);		//��������

	void Set(int nType);	//�ݒ菈��
	void Init(void);		//����������
	void Update(void);		//�X�V����
	int GetType(void) { return m_nType; }
	bool IsFinish(void);
	void SetInfo(INFO info) { m_aInfo[m_nType] = info; }
	void SetModel(CModelHier **ppModel, int nNumModel) { m_ppModel = ppModel; }		//���f���̏��ݒ�

	void FileLoad(void);

private:
	INFO m_aInfo[MOTIONTYPE_MAX];
	int m_nNumAll;
	int m_nType;		//���[�V�����̎��
	bool m_bLoop;		//���[�v���邩
	int m_NumKey;		//�L�[��
	int m_nKey;			//�L�[
	int m_nCounter;		//���[�V�����J�E���^�[
	bool m_bFinish;		//�I���������ǂ���
	
	CModelHier **m_ppModel;		//���f���ւ̃|�C���^
	int m_nNumModel;				//���f���̑���

	MOTIONTEXT m_textType;			//���[�V�����̃e�L�X�g�̎��
	static char *m_apFileName[MOTIONTEXT_MAX];		//�t�@�C����
};

#endif