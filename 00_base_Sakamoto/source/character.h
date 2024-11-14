//============================================
//
//	�L�����N�^�[�̏��� [character.h]
//	Author:sakai minato
//
//============================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "object.h"

class CModel;
class CMotion;

#define MODEL_NUM		(64)	// ���f���̐�
#define FILE_NAME_SIZE	(128)	// �t�@�C�����̍ő啶����

//�I�u�W�F�N�g�v���C���[�N���X
class CCharacter : public CObject
{
public:
	CCharacter();
	~CCharacter();

	static CCharacter* Create(const char* pModelName);
	HRESULT Init(const char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel* GetModel(int nCnt);
	CMotion* GetMotion(void);
	int GetNumModel(void) { return m_nNumModel; }

private:

	void LoadModel(const char* pFilename);

	CModel* m_apModel[MODEL_NUM];
	CMotion* m_pMotion;
	char m_aModelName[FILE_NAME_SIZE];
	int m_nNumModel;
};

#endif