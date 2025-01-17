//============================================
//
//	�����o���^UI [popUI.cpp]
//	Author:���������Y
//
//============================================
#include "popUI.h"
#include "renderer.h"
#include "manager.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

	const float SAMPLE_WIGHT = 50.0f;		//����
	const float SAMPLE_HEIGHT = 50.0f;		//�c��
	const char TEXTURE_PASS[] = "data\\TEXTURE\\UI\\popUI.png";

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CPopUi::CPopUi(int nPriority) : CObjectBillboard(nPriority)
{
	m_fStateCountParcent = 0.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CPopUi::~CPopUi()
{

}

//====================================================================
//��������
//====================================================================
CPopUi* CPopUi::Create(int nPriority)
{
	CPopUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CPopUi(nPriority);
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
HRESULT CPopUi::Init(void)
{
	CObjectBillboard::Init();

	//�e�N�X�`���ݒ�
	SetTexture(TEXTURE_PASS);

	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CPopUi::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CPopUi::Update(void)
{

	//���_���̍X�V
	CObjectBillboard::Update();

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
void CPopUi::Draw(void)
{
	CObjectBillboard::Draw();
}