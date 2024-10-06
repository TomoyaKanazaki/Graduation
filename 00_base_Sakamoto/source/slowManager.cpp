//======================================================================================
//
// �X���[����[slow.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "slowManager.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

// �ÓI�����o�ϐ��錾
CSlowManager* CSlowManager::m_apSlow[CAMP_MAX][MAX_SLOW_OBJ] = {};

//======================================================================================
// �R���X�g���N�^
//======================================================================================
CSlowManager::CSlowManager(ECamp camp, ETag tag)
{
	m_tag = tag;
	m_camp = camp;
	m_fValue = 1.0f;
	m_fCount = 0.0f;

	if (camp >= CAMP_MAX && camp < CAMP_NONE)
	{// �͈͊O�̏ꍇ�A�����Ȃ�
		m_camp = CAMP_NONE;
	}

	for (int nCntObject = 0; nCntObject < MAX_SLOW_OBJ; nCntObject++)
	{
		if (m_apSlow[m_camp][nCntObject] == nullptr)
		{
			m_apSlow[m_camp][nCntObject] = this;	//�������g����

			break;
		}
	}
}

//======================================================================================
// �f�X�g���N�^
//======================================================================================
CSlowManager::~CSlowManager()
{

}

//======================================================================================
// ����
//======================================================================================
CSlowManager* CSlowManager::Create(ECamp camp, ETag tag)
{
	CSlowManager* m_pSlow = nullptr;

	if (m_pSlow == nullptr)
	{
		m_pSlow = new CSlowManager(camp, tag);

		m_pSlow->Init();

		return m_pSlow;
	}

	return m_pSlow;
}

//======================================================================================
// ����������
//======================================================================================
HRESULT CSlowManager::Init()
{

	return S_OK;
}

//======================================================================================
// �S�j��
//======================================================================================
void CSlowManager::ReleaseAll(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			delete m_apSlow[nCntCamp][i];
			m_apSlow[nCntCamp][i] = nullptr;
		}
	}
}

//======================================================================================
// �S�̂��X���[�ɂ���
//======================================================================================
void CSlowManager::SetValueAll(float fValue)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// �l��ݒ�(�S��)
			m_apSlow[nCntCamp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// �S�̂�ʏ�֖߂�
//======================================================================================
void CSlowManager::SetValueDefault(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// �l���f�t�H���g(�S��)
			m_apSlow[nCntCamp][i]->SetValue(1.0f);
		}
	}
}

//======================================================================================
// �w�c�ʂ̐��l�ݒ�
//======================================================================================
void CSlowManager::SetValue(ECamp camp, float fValue)
{
	for (int i = 0; i < MAX_SLOW_OBJ; i++)
	{
		if (m_apSlow[camp][i] == nullptr)
		{
			continue;
		}

		if (m_apSlow[camp][i]->GetCamp() == camp)
		{
			// �l��ݒ�(�w�c�̃^�O)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// �w�c,�^�O�ʂ̐��l�ݒ�
//======================================================================================
void CSlowManager::SetValue(ECamp camp, ETag tag, float fValue)
{
	for (int i = 0; i < MAX_SLOW_OBJ; i++)
	{
		if (m_apSlow[camp][i] == nullptr)
		{
			continue;
		}

		if (m_apSlow[camp][i]->GetTag() == tag)
		{
			// �l��ݒ�(�w�c�̃^�O)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}
