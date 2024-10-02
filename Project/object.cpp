//============================================
//
//	�I�u�W�F�N�g [object.cpp]
//	Author:sakamoto kai
//
//============================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debugproc.h"
#include "input.h"

//�}�N����`
#define POLYGON_WIGHT (300.0f)	//����
#define POLYGON_HEIGHT (200.0f)	//�c��

//�ÓI�����o�ϐ��錾
CObject *CObject::m_pTop[PRIORITY_MAX] = {};
CObject *CObject::m_pCur[PRIORITY_MAX] = {};
int CObject::m_nNumAll = 0;					//�I�u�W�F�N�g����
bool CObject::m_bLevelStop = false;			//�I�u�W�F�N�g����

//====================================================================
//�R���X�g���N�^
//====================================================================
CObject::CObject(int nPriority)
{
	if (m_pTop[nPriority] == NULL)
	{//���̃I�u�W�F�N�g���ЂƂ��Ȃ���
		m_pTop[nPriority] = this;
		m_pPrev = nullptr;
	}
	else
	{
		m_pPrev = m_pCur[nPriority];
		m_pPrev->m_pNext = this;
	}

	m_Priority = nPriority;
	m_pCur[nPriority] = this;
	m_pNext = nullptr;

	m_bDeath = false;
	m_bLevelUI = false;
	m_Appear = true;
	m_nNumAll++;					//�������J�E���g�A�b�v
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CObject::~CObject()
{
	
}

//====================================================================
//�S�ẴI�u�W�F�N�g�̔j��
//====================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�I������
			pObject->Uninit();

			//�폜����
			pObject->SetDeathFlag(true);

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�폜����
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}

//====================================================================
//�S�ẴI�u�W�F�N�g�̍X�V
//====================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			if ((m_bLevelStop == false && pObject->m_bLevelUI == false) ||
				(m_bLevelStop == true && pObject->m_bLevelUI == true))
			{
				if (CManager::GetInstance()->GetPause() == true)
				{
					if (pObject->m_type != TYPE_BLOCK &&
						pObject->m_type != TYPE_TIME)
					{
						//�X�V����
						pObject->Update();
					}
				}
				else if (pObject->m_Appear == true)
				{
					//�X�V����
					pObject->Update();
				}
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�폜����
			pObject->Release();

			pObject = pObjectNext;
		}
	}

	DebugKey();

	CManager::GetInstance()->GetDebugProc()->Print("�I�u�W�F�N�g�̑���:[%d]\n", m_nNumAll);
}

//====================================================================
//�S�ẴI�u�W�F�N�g�̕`��
//====================================================================
void CObject::DrawAll(int nCnt)
{
	if (CManager::GetInstance()->GetGameSpeed() <= 0.1f)
	{
		MultiTargetDraw();
	}

	switch (nCnt)
	{
	case 0:

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject* pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

			while (pObject != NULL)
			{
				CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

				if (pObject->m_Appear == true)
				{
					//�`�揈��
					pObject->Draw();
				}

				pObject = pObjectNext;
			}
		}

		break;
	case 1:

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject* pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

			while (pObject != NULL)
			{
				CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

				if (pObject->m_Appear == true)
				{
					if (pObject->m_type == CObject::TYPE_PLAYER3D || 
						pObject->m_type == CObject::TYPE_CUBEBLOCK ||
						pObject->m_type == CObject::TYPE_OBJMESHFIELD ||
						pObject->m_type == CObject::TYPE_MAPMODEL || 
						pObject->m_type == CObject::TYPE_ENEMY3D)
					{
						//�`�揈��
						pObject->Draw();
					}
				}

				pObject = pObjectNext;
			}
		}

		break;
	}
}

//====================================================================
//�S�ẴI�u�W�F�N�g�̕`��
//====================================================================
void CObject::MultiTargetDraw(void)
{
	//-----------------------------------------------------------------------------------

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	D3DVIEWPORT9 viewportDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;

	//�J�����̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	m_pDevice->GetRenderTarget(0, &pRenderDef);

	//���݂�Z�o�b�t�@���擾(�ۑ�)
	m_pDevice->GetDepthStencilSurface(&pZBuffDef);

	//���݂̃r���[�|�[�g���擾(�ۑ�)
	m_pDevice->GetViewport(&viewportDef);

	CManager::GetInstance()->GetRenderer()->ChageTarget(pCamera->GetPosV(), pCamera->GetPosR(), pCamera->GetVecU());

	//�����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
	m_pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//-----------------------------------------------------------------------------------

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			if (pObject->m_bLevelUI == false || (m_bLevelStop == true && pObject->m_bLevelUI == true))
			{
				if (pObject->m_Appear == true)
				{
					if (pObject->GetType() != TYPE_PLAYER3D && pObject->GetType() != TYPE_SAMPLE)
					{
						//�`�揈��
						pObject->Draw();
					}
				}
			}

			pObject = pObjectNext;
		}
	}

	//-----------------------------------------------------------------------------------

	//���̃����_�����O�^�[�Q�b�g�ɖ߂�
	m_pDevice->SetRenderTarget(0, pRenderDef);

	//����Z�o�b�t�@�ɖ߂�
	m_pDevice->SetDepthStencilSurface(pZBuffDef);

	//���̃r���[�|�[�g�ɖ߂�
	m_pDevice->SetViewport(&viewportDef);

	//-----------------------------------------------------------------------------------
}

//====================================================================
//�I�u�W�F�N�g�̔j��
//====================================================================
void CObject::Release(void)
{
	if (m_bDeath == true)
	{
		if (m_pTop[m_Priority] == m_pCur[m_Priority])
		{//�I�u�W�F�N�g���P�����Ȃ���
			m_pTop[m_Priority] = NULL;
			m_pCur[m_Priority] = NULL;
		}
		else if (this == m_pCur[m_Priority])
		{//�������Ō���̎�
			m_pPrev->m_pNext = NULL;
			m_pCur[m_Priority] = m_pPrev;
		}
		else if (this == m_pTop[m_Priority])
		{//�������擪�̎�
			m_pNext->m_pPrev = NULL;
			m_pTop[m_Priority] = m_pNext;
		}
		else
		{//�擪�ł��Ō���ł��Ȃ���
			m_pPrev->m_pNext = m_pNext;
			m_pNext->m_pPrev = m_pPrev;
		}

		this->SetNULL();
		delete this;
		m_nNumAll--;				//�������J�E���g�_�E��
	}
}

//====================================================================
// �f�o�b�O�L�[
//====================================================================
void CObject::DebugKey()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{//F5�L�[�������ꂽ�Ƃ�
	
		//UI�n�I�u�W�F�N�g�̕`�� / ��`��
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject* pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

			while (pObject != NULL)
			{
				CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

				if (
					pObject->m_type == TYPE_2DUI ||
					pObject->m_type == TYPE_LOG ||
					pObject->m_type == TYPE_OBJECT2D ||
					pObject->m_type == TYPE_TIME ||
					pObject->m_type == TYPE_NUMBER
					)
				{
					pObject->SetAppear(!pObject->GetAppear());
				}

				pObject = pObjectNext;
			}
		}
	}
}

//====================================================================
//�I�u�W�F�N�g�̔j��
//====================================================================
void CObject::ResetObjectMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			if (
				pObject->m_type == TYPE_PLAYER3D ||
				pObject->m_type == TYPE_BREAKBLOCK3D ||
				pObject->m_type == TYPE_ENEMY3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�폜����
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}

//====================================================================
//�G�̍폜
//====================================================================
void CObject::ReleaseEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			if (
				pObject->m_type == TYPE_ENEMY3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�폜����
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}


//====================================================================
//�u���b�N�̔j��
//====================================================================
void CObject::DeleteBlock(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			if (
				pObject->m_type == TYPE_BREAKBLOCK3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			//�폜����
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}
