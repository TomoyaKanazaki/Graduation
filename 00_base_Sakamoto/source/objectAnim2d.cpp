//============================================
//
//	�I�u�W�F�N�g�A�j���[�V����2D [objectAnim2d.cpp]
//	Author:�O�엳�V��
//
//============================================
#include "objectAnim2d.h"

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CObjectAnim2D>* CObjectAnim2D::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CObjectAnim2D::CObjectAnim2D(int nPriority) :CObject2D(nPriority)
{
	m_nU = 0;//�������̖���
	m_nV = 0;//�c�����̖���
	m_nAnim = 0;//�A�j���[�V�����̍ő喇��
	m_nCurrent = 0;//���݂̖���
	m_fFramerate = 0; //�X�V���x
	m_fCurrentFrame = 0;
	m_bLoop = 0;//���[�v�̗L��
	m_bReverse = 0; //�t�Đ��̗L��
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CObjectAnim2D::~CObjectAnim2D()
{

}

//====================================================================
//��������
//====================================================================
CObjectAnim2D* CObjectAnim2D::Create(D3DXVECTOR3 pos, int nU, int nV, int nAnim, bool bLoop, float fRate, int nPriority)
{
	CObjectAnim2D* pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObjectAnim2D(nPriority);
	}
	pObject2D->Init();
	pObject2D->SetPos(pos);
	pObject2D->m_nU = nU;
	pObject2D->m_nV = nV;
	pObject2D->m_nAnim = nAnim;
	pObject2D->m_bLoop = bLoop;
	pObject2D->m_fFramerate = fRate;
	return pObject2D;
}

//====================================================================
//����������
//====================================================================
HRESULT CObjectAnim2D::Init(void)
{	
	CObject2D::Init();

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CObjectAnim2D>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CObjectAnim2D::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CObjectAnim2D::Update(void)
{
	SetVerTex();
	if (m_nCurrent < m_nAnim)
	{
		m_fCurrentFrame += 1.0f;
		if (m_fCurrentFrame > (60.0f / m_fFramerate))
		{
			m_fCurrentFrame -= (60.0f / m_fFramerate);
			m_nCurrent++;
		}
	}
	else if (m_bLoop)
	{
		m_nCurrent = 0;
	}
	
	SetAnimTex(m_nCurrent, m_nU, m_nV);
}

//====================================================================
//�`�揈��
//====================================================================
void CObjectAnim2D::Draw(void)
{

	CObject2D::Draw();
}

//==========================================
// ���X�g�̎擾
//==========================================
CListManager<CObjectAnim2D>* CObjectAnim2D::GetList(void)
{
	return m_pList;
}
