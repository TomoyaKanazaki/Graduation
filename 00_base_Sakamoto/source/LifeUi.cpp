//============================================
//
//	�I�u�W�F�N�g2D�̃T���v�� [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "LifeUi.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float SAMPLE_WIGHT = 70.0f;		//����
	const float SAMPLE_HEIGHT = 70.0f;		//�c��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CLifeUi::CLifeUi(int nPriority) : CObject2D(nPriority)
{
	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);
	m_nSample = 0;
	m_pNumber = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CLifeUi::~CLifeUi()
{

}

//====================================================================
//��������
//====================================================================
CLifeUi* CLifeUi::Create(int nPriority)
{
	CLifeUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CLifeUi(nPriority);
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
HRESULT CLifeUi::Init(void)
{
	CObject2D::Init();

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\player\\life.png");

	m_pNumber = CNumber::Create();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CLifeUi::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CLifeUi::Update(void)
{
	//���_���̍X�V
	CObject2D::Update();

	//----���_����ύX�������ꍇ�͈ȉ��̃R�[�h���g�p���邱��-----
	//VERTEX_2D* pVtx;	//���_�|�C���^������

	////���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////�ʒu
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////�F
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////�e�N�X�`��
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////���_�o�b�t�@���A�����b�N����
	//GetVtxBuff()->Unlock();
}

//====================================================================
//�`�揈��
//====================================================================
void CLifeUi::Draw(void)
{
	CObject2D::Draw();
}