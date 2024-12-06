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

	int Regist(CObjectCharacter* pObjCharacter, const char* pFilename);
	const char* GetCharacterName(int Idx) { return &m_aCharacterInfo[Idx].acFileName[0]; }

private:

	// �}�N����`
#define MAX_LENGTH_TXT	(128)	// �e�L�X�g�̍ő啶����
#define MAX_CHARACTER	(128)	// �L�����N�^�[�̍ő吔
#define MAX_MODEL_PARTS	(32)	// ���f���p�[�c���̍ő吔
#define MAX_MOTION		(32)	// ���[�V�������̍ő吔
#define MAX_KEY			(16)	// �L�[���̍ő吔

	// ���f���p�[�c���
	struct ModelParts
	{
		char acModelFileName[MAX_LENGTH_TXT];	// �t�@�C����

		int nIndex;			// �C���f�b�N�X
		int nParent;		// �e�̔ԍ�

		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	};

	// ���f���Ǘ����
	struct ModelManager
	{
		int nNumModel;								// ���f����
		ModelParts aModelParts[MAX_MODEL_PARTS];	// �p�[�c���
	};

	// �L�[���
	struct Key
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	};

	// �L�[�Ǘ����
	struct KeyManager
	{
		int nFrame;					// �Đ��t���[��
		Key aKey[MAX_MODEL_PARTS];	// �e���f���̃L�[�v�f
	};

	// ���[�V�������
	struct MotionInfo
	{
		bool bLoop;							// ���[�v�̗L��
		int nNumKey;						// �L�[�̑���
		KeyManager aKeyManager[MAX_KEY];	// �L�[�Ǘ����
	};

	// ���[�V�����Ǘ����
	struct MotionManager
	{
		int nNumMotion;						// ���[�V������
		MotionInfo aMotionInfo[MAX_MOTION];	// ���[�V�������
	};

	// �L�����N�^�[���
	struct CharacterInfo
	{
		ModelManager ModelManager;			// ���f���Ǘ����
		MotionManager MotionManager;		// ���[�V�����Ǘ����

		char acFileName[MAX_CHARACTER];		// �t�@�C����
	};

	void SetModelData(CObjectCharacter* pObjCharacter, int nNumCharacter);
	void SetMotionData(CObjectCharacter* pObjCharacter, int nNumCharacter);

	bool LoadModel(const char* pFileName, int nNumCharacter);
	bool LoadMotion(const char* pFileName, int nNumModel, int nNumCharacter);

	CharacterInfo m_aCharacterInfo[MAX_CHARACTER];
	int m_nNumAll;						// �L�����N�^�[�̎g�p��

};
#endif