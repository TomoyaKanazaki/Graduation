//========================================
//
//���[�V����[motion.h]
//Author�F�X��x��
//
//========================================
#ifndef _MOTION_H_
#define _MOTION_H_
#include "main.h"
#include "model.h"

//========================================
// �萔��`
//========================================
namespace
{
const int MAX_MOTION = 8;	// ���[�V�����̍ő吔
const int MAX_MODEL = 16;	// �p�[�c�̍ő吔
const int MAX_KEY = 32;		// �L�[�̍ő吔
}

//========================================
//���[�V�����N���X
//========================================
class CMotion
{
public:
	enum PlayerMotion
	{// �v���C���[���[�V�������
		PLAYER_MOTIONTYPE_NEUTRAL = 0,	// �ҋ@
		PLAYER_MOTIONTYPE_WALK,			// �ړ�
		PLAYER_MOTIONTYPE_JUMP,			// �W�����v
		PLAYER_MOTIONTYPE_ROLL,			// �]����
		PLAYER_MOTIONTYPE_MAX
	};

	enum EnemyMotion
	{// �G�l�~�[���[�V�������
		ENEMY_MOTIONTYPE_WALK = 0,	// �ړ�
		ENEMY_MOTIONTYPE_ATTACK,	// �U��
		ENEMY_MOTIONTYPE_MAX
	};

	struct KEY
	{//�L�[�̍\����
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};	//�ʒu�E����

	struct KEY_INFO
	{//�L�[���̍\����
		int nFrame;				//�Đ��t���[��
		KEY aKey[MAX_MODEL];	//�e���f���̏��
	};

	struct INFO
	{//���[�V�������̍\����
		bool bLoop;		//���[�v���邩�ǂ���
		int nNumKey;	//�L�[�̑���
		KEY_INFO aKeyInfo[MAX_KEY];	//�L�[���
	};

	CMotion();		// �R���X�g���N�^
	~CMotion();		// �f�X�g���N�^

	//�����o�֐�
	static CMotion *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetInfo(int nType);
	void Set(int nType);
	void SetModel(CModel *ppModel, int nNumModel);
	CModel** GetModel(void) { return &m_ppModel[0]; }	// ���f���̏��擾
	CModel* GetModel(int nIdx) { return m_ppModel[nIdx]; }

	int GetMotionKey(int nType) { return m_aInfo[nType].nNumKey; }	// �Đ������[�V�����L�[�̑����擾
	int GetNowKey() { return m_nKey; }	// ���݂̃L�[
	int GetType() { return m_nType; }	// ��ނ�Ԃ�

	bool IsLoop(int nType) { return m_aInfo[nType].bLoop; }	//	���[�v���邩���Ȃ���
	bool IsFinish() { return m_bFinish; }	// �I���������ǂ�����Ԃ�

	void Load(std::string pfile);

private:
	//�����o�ϐ�
	int m_nNumAll;		// ���[�V��������
	int m_nType;		// ���[�V�������
	int m_nKey;			// ���݂̃L�[�i���o�[
	int m_nCounter;		// ���[�V�����J�E���^�[
	int m_nNumMotion;	// ���݂̃��[�V����
	int m_nNumParts;	// �p�[�c��
	int m_nPartsIndex;	// �p�[�c�̔ԍ�
	int m_nNowKey;		// ���݂̃L�[
	int m_nNumModel;	// ���f���̑���
	bool m_bFinish;		// �I���������ǂ���

	INFO m_aInfo[MAX_MOTION];		// ���[�V�������
	CModel *m_ppModel[MAX_PARTS];	// ���f���ւ̃_�u���|�C���^
	CModel *m_pParent;				// �e���f���ւ̃|�C���^
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot;				// ����
};

#endif
