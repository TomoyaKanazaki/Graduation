//============================================
//
//	�����̏��� [number.cpp]
//	Author:sakamoto kai
//
//============================================
#include "number.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float NUMBER_WIGHT = 100.0f;		//����
	const float NUMBER_HEIGHT = 100.0f;		//�c��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CNumber::CNumber(int nPriority) : CObject2D(nPriority)
{
	m_nNumber = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CNumber::~CNumber()
{

}

//====================================================================
//��������
//====================================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pNumber = new CNumber();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pNumber->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pNumber;
}

//====================================================================
//����������
//====================================================================
HRESULT CNumber::Init(void)
{
	CObject2D::Init();

	SetSize(D3DXVECTOR3(NUMBER_WIGHT, NUMBER_HEIGHT, 0.0f));

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\Number_02.png");

	//�^�C�v�ݒ�
	SetType(CObject::TYPE_NUMBER);

	//�A�j���[�V�����̏����l
	SetAnim(D3DXVECTOR2(0.2f, 0.2f));

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CNumber::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CNumber::Update(void)
{
	VERTEX_2D*pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̍X�V
	CObject2D::Update();

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	GetVtxBuff()->Unlock();
}

//====================================================================
//�`�揈��
//====================================================================
void CNumber::Draw(void)
{
	CObject2D::Draw();
}