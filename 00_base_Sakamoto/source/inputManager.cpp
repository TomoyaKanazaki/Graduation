//============================================
//
//	���͊Ǘ� [inputManager.h]
//	Author: Kai Takada
//
//============================================

#include "inputManager.h"

//�萔��`
namespace
{

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CInputManager::CInputManager()
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CInputManager::~CInputManager()
{

}

//====================================================================
//��������
//====================================================================
CInputManager* CInputManager::Create(void)
{
	CInputManager* pInstance = NULL;

	if (pInstance == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pInstance = new CInputManager();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CInputManager::Init(void)
{
	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CInputManager::Uninit(void)
{

}