//===============================================================================
//
// �X���[����[slowManajer.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SLOWMANAGER_H_
#define _SLOWMANAGER_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �X���[�}�l�[�W���N���X
class CSlowManager
{
private:
	static const int MAX_SLOW_OBJ = 24;	// �ő吔

public:
	enum ECamp	// �w�c
	{
		CAMP_NONE,
		CAMP_PLAYER,
		CAMP_ENEMY,
		CAMP_MAX
	};

	enum ETag
	{
		TAG_NONE = 0,	// �w�c�Ȃ�
		TAG_PLAYER,		// �v���C���[�w�c
		TAG_ENEMY,		// �G�w�c
		TAG_MAX
	};

public:
	CSlowManager(ECamp nCamp = CAMP_NONE, ETag tag = TAG_NONE);	// �R���X�g���N�^
	~CSlowManager();	// �f�X�g���N�^

	static CSlowManager* Create(ECamp camp = CAMP_NONE, ETag tag = TAG_NONE);

	// �v���g�^�C�v�錾
	HRESULT Init();

	static void ReleaseAll(void);

	// �ݒ�
	void SetTag(ETag tag) { m_tag = tag; }	// �^�O
	void SetCamp(ECamp camp) { m_camp = camp; }	// �����w�c
	void SetValue(float fValue) { m_fValue = fValue; }
	static void SetValue(ECamp camp, float fValue);
	static void SetValue(ECamp camp, ETag tag, float fValue);
	static void SetValueAll(float fValue);	// �S�̂��X���[
	static void SetValueDefault(void);	// �f�t�H���g

	// �擾
	ETag GetTag(void) const { return m_tag; }
	ECamp GetCamp(void) const { return m_camp; }
	float GetValue(void) const { return m_fValue; }
	CSlowManager** GetInfo(void) { return &m_apSlow[0][0]; }

private:
	// �����o�ϐ�
	ETag m_tag;	// �^�O
	float m_fValue;	// ���l
	float m_fCount;	// �J�E���g�p

	// ���
	static CSlowManager* m_apSlow[CAMP_MAX][MAX_SLOW_OBJ];	// �Ǘ��p
	ECamp m_camp;	// ����
};

#endif