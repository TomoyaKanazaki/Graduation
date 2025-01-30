//============================================
//
//	�l���X�R�AUI [getscoreUi.cpp]
//	Author:���������Y
//
//============================================
#include "getscoreUi.h"
#include "numBillboard.h"
#include "renderer.h"
#include "manager.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int OBJ_LIFE = 50;

	const float OBJ_DEGITPOS_X =35.0f;

	const float OBJ_ADDPOS_Y = 60.0f;
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CGetscoreUi::CGetscoreUi(int nPriority) : CObject(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		m_pNum[nCnt] = nullptr;
	}
	m_nLife = OBJ_LIFE;

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGetscoreUi::~CGetscoreUi()
{

}

//====================================================================
//��������
//====================================================================
CGetscoreUi* CGetscoreUi::Create(int nGetScore,D3DXVECTOR3 pos,int nPriority)
{
	CGetscoreUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CGetscoreUi(nPriority);
	}

	int nScore = nGetScore;
	bool bDegitHead = false;

	for (int nCnt = MAX_NUM-1; nCnt >= 0; nCnt--)
	{
		int nDegit = pow(10, nCnt);
		if (nScore / nDegit >= 1|| bDegitHead==true)
		{
			bDegitHead = true;
			pSample2D->m_pNum[nCnt] =  CNumBillboard::Create();	//�����r���{�[�hobj�̐���
			pSample2D->m_pNum[nCnt]->SetNumber(nScore / nDegit);//�\�����鐔�̐ݒ�

			pSample2D->m_pNum[nCnt]->SetPos(D3DXVECTOR3(pos.x - nCnt * OBJ_DEGITPOS_X + OBJ_DEGITPOS_X * 2, pos.y + OBJ_ADDPOS_Y, pos.z+50.0f));

			nScore = nScore - nScore / nDegit * nDegit;//���݂̌������폜
		}
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample2D->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSample2D;
}

//====================================================================
//����������
//====================================================================
HRESULT CGetscoreUi::Init(void)
{
	
	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGetscoreUi::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			m_pNum[nCnt]->Uninit();
			m_pNum[nCnt] = nullptr;
		}
	}
}

//====================================================================
//�X�V����
//====================================================================
void CGetscoreUi::Update(void)
{
	
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			D3DXVECTOR3 posold = m_pNum[nCnt]->GetPos();
			m_pNum[nCnt]->SetPos(D3DXVECTOR3(posold.x, posold.y+ 0.3f, posold.z + 0.3f));
		}
	}
	//�����J�E���g�_�E��
	m_nLife--;
	
	if (m_nLife <= 0)
	{//�������؂ꂽ�ꍇ�폜
		this->Uninit();
	}
}
//====================================================================
//�`�揈��
//====================================================================
void CGetscoreUi::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			m_pNum[nCnt]->Draw();
		}
	}
}