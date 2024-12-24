//============================================
//
//	�L�����N�^�[�̊Ǘ� [characterManager.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "objectcharacter.h"

// �L�����N�^�[�Ǘ��N���X
class CCharacterManager
{
public:

	CCharacterManager();
	~CCharacterManager();

	HRESULT Load(void);
	void Unload(void);

	int Regist(CObjectCharacter* pObjCharacter, const std::string pFilename);
	const char* GetCharacterName(int Idx) { return &m_aCharacterInfo[Idx].acFileName[0]; }

private:

	// �}�N����`
	#define MAX_LENGTH_TXT	(128)	// �e�L�X�g�̍ő啶����
	#define MAX_CHARACTER	(128)	// �L�����N�^�[�̍ő吔
	#define MAX_MODEL_PARTS	(32)	// ���f���p�[�c���̍ő吔
	#define MAX_MOTION		(32)	// ���[�V�������̍ő吔
	#define MAX_KEY			(16)	// �L�[���̍ő吔

	// ���f���p�[�c���
	struct SModelParts
	{
		char acModelFileName[MAX_LENGTH_TXT];	// �t�@�C����

		int nIndex;			// �C���f�b�N�X
		int nParent;		// �e�̔ԍ�

		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	};

	// ���f���Ǘ����
	struct SModelManager
	{
		int nNumModel;				// ���f����
		SModelParts* aModelParts;	// �p�[�c���
	};

	// �L�[���
	struct SKey
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	};

	// �L�[�Ǘ����
	struct SKeyManager
	{
		int nFrame;					// �Đ��t���[��
		SKey aKey[MAX_MODEL_PARTS];	// �e���f���̃L�[�v�f
	};

	// ���[�V�������
	struct SMotionInfo
	{
		bool bLoop;							// ���[�v�̗L��
		int nNumKey;						// �L�[�̑���
		SKeyManager aKeyManager[MAX_KEY];	// �L�[�Ǘ����
	};

	// ���[�V�����Ǘ����
	struct SMotionManager
	{
		int nNumMotion;						// ���[�V������
		SMotionInfo aMotionInfo[MAX_MOTION];	// ���[�V�������
	};

	// �L�����N�^�[���
	struct SCharacterInfo
	{
		SModelManager ModelManager;			// ���f���Ǘ����
		SMotionManager MotionManager;		// ���[�V�����Ǘ����

		char acFileName[MAX_CHARACTER];		// �t�@�C����
	};

	void SetModelData(CObjectCharacter* pObjCharacter, int nNumCharacter);
	void SetMotionData(CObjectCharacter* pObjCharacter, int nNumCharacter);

	bool LoadModel(const std::string pFileName, int nNumCharacter);
	bool LoadMotion(const std::string pFileName, int nNumModel, int nNumCharacter);

	SCharacterInfo m_aCharacterInfo[MAX_CHARACTER];
	int m_nNumAll;						// �L�����N�^�[�̎g�p��

};
#endif