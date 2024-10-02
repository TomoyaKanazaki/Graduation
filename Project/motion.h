//============================================
//
//	���[�V���� [motion.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MOTION_H_
#define _MOTION_H_

//�}�N����`
#define MAX_INFO (64)	//���̍ő吔
#define MAX_PARTS (64)	//�p�[�c�̑���
#define MAX_KEY (32)	//�L�[�̑���

//�O���錾
class CModel;

//�}�b�v�N���X
class CMotion
{
public:
	CMotion();
	~CMotion();

	//�L�[�̍\����
	typedef struct
	{
		float fPosX;		//�ʒuX
		float fPosY;		//�ʒuY
		float fPosZ;		//�ʒuZ
		float fRotX;		//����X
		float fRotY;		//����Y
		float fRotZ;		//����Z
	}KEY;

	//�L�[���̍\����
	typedef struct
	{
		int nFrame;					//�Đ��t���[��
		KEY aKey[MAX_PARTS];		//�e���f���̃L�[�v�f[�p�[�c�̑���]
	}KEY_INFO;

	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;						//���[�v���邩�ǂ���
		int NumKey;						//�e���f���̃L�[�v�f
		KEY_INFO aKeyInfo[MAX_KEY];		//�L�[���[�L�[�̑���]
	}INFO;

	void Set(int nType, float BlendTime);
	void SetStopPose(int nType, int nKey, float nCounter);
	void Update(void);
	void SetModel(CModel **ppModel, int nNumModel);
	void SetSlowVaule(float fValue) { m_fSlowVaule = fValue; }
	void LoadData(const char* pFilename);
	int GetType(void) { return m_nType; }
	int GetKey(void) { return m_nKey; }
	float GetCounter(void) { return m_fCounter; }
	float GetCounterCurrent(void) { return m_fCounterCurrent; }
	bool GetFinish(void) { return m_bFinish; }
	bool GetLoop() { return m_bLoop; }
	int GetFullFrame(int nIdx);

private:
	INFO m_aInfo[MAX_INFO];		//���[�V�����̏��[���[�V�����̑���]
	int m_nNumAll;				//���f��(�p�[�c)�̑���
	int m_nType;				//���[�V�����̎��
	bool m_bLoop;				//���[�v���邩�ǂ���
	int m_nNumKey;				//�L�[�̑���
	int m_nKey;					//���݂̃L�[No.
	int m_nNextKey;				//���݂̎��̃L�[No.
	float m_fCounter;			//���[�V�����̃J�E���^�[
	float m_fCounterCurrent;	//���[�V�����̌��݂̃J�E���^�[
	int m_nBlendType;			//���[�V�����u�����h�p�̎��
	int m_nBlendKey;			//���[�V�����u�����h�p�̃L�[No.
	float m_nBlendCounter;		//���[�V�����u�����h�p�̃J�E���^�[
	float m_nBlendTime;			//���[�V�����u�����h�̎���
	float m_nBlendTimeMax;		//���[�V�����u�����h�̎���
	float m_fSlowVaule;			//�X���[(�J�E���g�Ɋ|����p)
	bool m_bFinish;				//�I���������ǂ���

	CModel **m_ppModel;			//���f���ւ̃|�C���^
	int m_nNumModel;			//���f���̑���
};
#endif