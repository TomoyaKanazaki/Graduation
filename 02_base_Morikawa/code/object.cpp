//========================================
//
//�I�u�W�F�N�g�Ǘ�[object.cpp]
//Author�F�X��x��
//
//========================================
#include "object.h"
#include "camera.h"
#include "manager.h"
#include "texture.h"

//========================================
//�ÓI�����o�ϐ��錾
//========================================
int CObject::m_nNumAll = 0;
CObject *CObject::m_pTop[NUM_PRIORITY] = {};
CObject *CObject::m_pCurrent[NUM_PRIORITY] = {};

//========================================
//�R���X�g���N�^
//========================================
CObject::CObject(int nPriority)
{//�l�N���A
	if (m_pTop[nPriority] == nullptr && m_pCurrent[nPriority] == nullptr)
	{//�擪�ƍŌオnullptr�̎�
		m_pTop[nPriority] = this;	//���g����
		m_nNumAll++;				//�������J�E���g�A�b�v
		m_nPriority = nPriority;	// �D�揇��
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		m_pNext = nullptr;	//��
		m_pPrev = nullptr;	//�O
		m_pCurrent[nPriority] = this;	//���g����
		m_bRelease = false;	//�I���t���O
		m_bDrawFrag = false;
	}
	else if (m_pTop[nPriority] != nullptr && m_pCurrent[nPriority] != nullptr)
	{
		m_pPrev = m_pCurrent[nPriority];
		m_pNext = nullptr;
		m_pPrev->m_pNext = this;
		m_pCurrent[nPriority] = this;
	}
}

//========================================
//�f�X�g���N�^
//========================================
CObject::~CObject(void)
{
}

//========================================
//���ׂẴI�u�W�F�N�g�̔j��
//========================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// �擪���
		CObject* Now = m_pTop[nCntPri];

		CObject* Next;

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// ���̒l��ۑ�
				Next = Now->m_pNext;

				if (Now->m_bDrawFrag != true)
				{
					// �擪�̏I��
					Now->Uninit();
				}
				else
				{
					int n = 0;
				}
				
				// ��
				Now = Next;
			}
		}
	}

	// ���ׂč폜
	DeleteAll();
}

//========================================
//���ׂẴI�u�W�F�N�g�̍X�V
//========================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// �擪���
		CObject *Now = m_pTop[nCntPri];

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				CObject* pNext = Now->m_pNext;

				// �擪�̍X�V
				if (Now->m_bRelease != true)
				{
					Now->Update();
				}

				// ��
				Now = pNext;
			}
		}		
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// �擪���
		CObject* pNow = m_pTop[nCntPri];

		while (1)
		{
			if (pNow == nullptr)
			{
				break;
			}
			else if (pNow != nullptr)
			{
				// next��m_pTop��next����
				CObject* pNext = pNow->m_pNext;

				if (pNow->m_bRelease == true)
				{
					if (pNow->m_bDrawFrag != true)
					{
						// �擪�̍폜
						pNow->Delete();
					}
					else
					{
						int n = 0;
					}
				}

				// ��
				pNow = pNext;
			}
		}
	}
}

//========================================
//���ׂẴI�u�W�F�N�g�̕`��
//========================================
void CObject::DrawAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// �擪���
		CObject* Now = m_pTop[nCntPri];

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				// ���ɐ擪�̎�����
				CObject* pNext = Now->m_pNext;

				// �擪�̍X�V
				if (Now->m_bRelease != true)
				{
					if (Now->m_bDrawFrag != true)
					{
						// �擪�̕`��
						Now->Draw();
					}
				}

				// ��
				Now = pNext;
			}
		}
	}
}

//========================================
//���ׂẴI�u�W�F�N�g�̏I��
//========================================
void CObject::DeleteAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		// �擪���
		CObject* Now = m_pTop[nCntPri];

		CObject* Next;

		while (1)
		{
			if (Now == nullptr)
			{
				break;
			}
			else if (Now != nullptr)
			{
				//��
				Next = Now->m_pNext;

				if (Now->m_bRelease == true)
				{
					//�擪�̏I��
					Now->Delete();
				}

				//Next����
				Now = Next;
			}
		}
	}
}

//========================================
//���g�̍폜
//========================================
void CObject::Delete(void)
{
	// �擪���
	CObject *Now = this;
	CObject *pNextObj = Now->m_pNext;	// ���ۑ�
	CObject *pPrevObj = Now->m_pPrev;	// �O��ۑ�

	if (pNextObj != nullptr)
	{// next�̑O��Ɏ��g�̑O����
		pNextObj->m_pPrev = Now->m_pPrev;
	}

	if (pPrevObj != nullptr)
	{// prev�̎���Ɏ��g�̎�����
		pPrevObj->m_pNext = Now->m_pNext;
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		if (Now == m_pTop[nCntPri])
		{// �擪�Ɏ��g�̎�����
			m_pTop[nCntPri] = Now->m_pNext;
		}

		if (Now == m_pCurrent[nCntPri])
		{// �Ō���Ɏ��g�̑O����
			m_pCurrent[nCntPri] = Now->m_pPrev;
		}
	}

	// ���g�̍폜
	delete this;
}

//========================================
//�I�u�W�F�N�g�̔j��
//========================================
void CObject::Release(void)
{
	// ���S�t���O
	m_bRelease = true;
}

//========================================
// �ʒu�̎擾
//========================================
void CObject::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//========================================
// �ʒu�̎擾
//========================================
D3DXVECTOR3 CObject::GetPos(void)
{
	return m_pos;
}

//========================================
// �ʒu�̎擾
//========================================
void CObject::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//========================================
// �ړ��ʂ̎擾
//========================================
D3DXVECTOR3 CObject::GetMove()
{
	return m_move;
}

//========================================
// �ʒu�̎擾
//========================================
void CObject::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//========================================
// �����̎擾
//========================================
D3DXVECTOR3 CObject::GetRot(void)
{
	return m_rot;
}

//========================================
// �e�N�X�`���̊��蓖��
//========================================
void CObject::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
// �`�悷�邩���Ȃ���
//========================================
void CObject::SetDrawFrag(bool bDraw)
{
	m_bDrawFrag = bDraw;
}
