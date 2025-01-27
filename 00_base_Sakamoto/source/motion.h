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
private:

	// �Đ����@�񋓌^
	enum PLAYTYPE
	{
		PLAYTYPE_NORMAL = 0,
		PLAYTYPE_BLEND,
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

		// �R���X�g���N�^
		KEY() : fPosX(0.0f), fPosY(0.0f), fPosZ(0.0f),
			fRotX(0.0f), fRotY(0.0f), fRotZ(0.0f) {}
	};

	//�L�[���̍\����
	struct KEY_INFO
	{
		int nFrame;					// �Đ����t���[��
		KEY aKey[MAX_PARTS];		// �e���f���̃L�[�v�f[�p�[�c�̑���]

		// �R���X�g���N�^
		KEY_INFO() : nFrame(0), aKey() {}
	};

	//���[�V�������̍\����
	struct INFO
	{
		bool bLoop;						// ���[�v���邩�ǂ���
		int NumKey;						// �L�[�g�p����
		KEY_INFO aKeyInfo[MAX_KEY];		// �L�[���[�L�[�̑���]

		// �R���X�g���N�^
		INFO() : bLoop(false), NumKey(0), aKeyInfo() {}
	};

	// �g�p�K�w�\�����f���Ǘ��p�̍\����
	struct SBodyData
	{
		CModel** ppModel;		//���f���ւ̃|�C���^
		int nNumModel;			//���f���̑���

		// �R���X�g���N�^
		SBodyData() : ppModel(nullptr), nNumModel(0) {}
	};

	// ���[�V�����u�����h�p�̍\����
	struct SBlendInfo
	{
		int nOldId;				// �u�����h�O�̑O��̃��[�V����
		int nOldKey;			// �u�����h�O�̑O��̃L�[
		float fCounter;			// �o�߃J�E���g
		float fTime;			// �c��J�E���g
		float fTimeMax;			// ���J�E���g

		// �R���X�g���N�^
		SBlendInfo() : nOldId(0), nOldKey(0),
			fCounter(0.0f), fTime(0.0f), fTimeMax(0.0f) {}
	};

	// ���[�V�����Đ��p�̍\����
	struct SPlayInfo
	{
		int nId;		// ���݂̃��[�V�����ԍ�
		int nKey;		// ���݂̃L�[
		int nKeyNext;	// ���̃L�[
		float fCounter;	// �Đ��J�E���g
		bool bFinish;	// �I���t���O
		SBlendInfo blendInfo;	// �u�����h�p���
		PLAYTYPE type;		// �Đ����@�̎��

		// �R���X�g���N�^
		SPlayInfo() : nId(0), nKey(0), nKeyNext(1),
			fCounter(0.0f), bFinish(false), blendInfo(SBlendInfo()), type(PLAYTYPE::PLAYTYPE_NORMAL) {}
	};

public:

	CMotion();
	~CMotion();

	// �����o�֐�
	void Update(void);
	void Set(int nType, float BlendTime);
	void SetModel(CModel** ppModel, int nNumModel);
	void SetSlowVaule(float fValue) { m_fSlowVaule = fValue; }
	void LoadData(const std::string pFilename);

	// �����o�֐�(�擾�n)
	int GetType(void) { return m_PlayInfo.nId; }
	int GetKey(void) { return m_PlayInfo.nKey; }
	float GetCounter(void) { return m_PlayInfo.fCounter; }
	bool GetFinish(void) { return m_PlayInfo.bFinish; }
	bool GetInfoLoop(int nMotion);
	int GetInfoNumKey(int nMotion);
	int GetInfoKeyFrame(int nMotion, int nKey);

	// �����o�֐�(�ݒ�n)
	void SetInfoLoop(bool bLoop, int nMotion) { m_aInfo[nMotion].bLoop = bLoop; }
	void SetInfoNumKey(int nNumKey, int nMotion) { m_aInfo[nMotion].NumKey = nNumKey; }
	void SetInfoKeyFrame(int nFrame, int nMotion, int nKey) { m_aInfo[nMotion].aKeyInfo[nKey].nFrame = nFrame; }

	// �L�[���ݒ�֐� ( �p�����[�^, ���[�V�����ԍ�, �L�[�ԍ�, �p�[�c�ԍ�)
	void SetInfoPartsPosX(float fPosX, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosX = fPosX; }
	void SetInfoPartsPosY(float fPosY, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosY = fPosY; }
	void SetInfoPartsPosZ(float fPosZ, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fPosZ = fPosZ; }
	void SetInfoPartsRotX(float fRotX, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotX = fRotX; }
	void SetInfoPartsRotY(float fRotY, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotY = fRotY; }
	void SetInfoPartsRotZ(float fRotZ, int nMotion, int nKey, int nParts) { m_aInfo[nMotion].aKeyInfo[nKey].aKey[nParts].fRotZ = fRotZ; }

	int GetFullFrame(int nIdx);

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CMotion::* PLAYTYPE_FUNC)();
	static PLAYTYPE_FUNC m_PlayTypeFunc[];

	// ��Ԑݒ�֐�
	void PlayTypeNormal();
	void PlayTypeBlend();

	// �����o�ϐ�
	INFO m_aInfo[MAX_INFO];		// ���[�V�����̏��[���[�V�����̑���]
	SPlayInfo m_PlayInfo;		// ���[�V�����Đ��p�̏��
	SBodyData m_BodyData;		// �K�w���f���̏��
	float m_fSlowVaule;			//�X���[(�J�E���g�Ɋ|����p)
};
#endif