//============================================
//
//	�I�u�W�F�N�g2D�̃T���v�� [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "crossUI.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

	const float SAMPLE_WIGHT = 125.0f;		//����
	const float SAMPLE_HEIGHT = 125.0f;		//�c��
	const char TEXTURE_PASS[] = "data\\TEXTURE\\UI\\cross_UI.png";

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CCrossUi::CCrossUi(int nPriority) : CObject2D(nPriority)
{
	m_fStateCountParcent = 0.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCrossUi::~CCrossUi()
{

}

//====================================================================
//��������
//====================================================================
CCrossUi* CCrossUi::Create(int nPriority)
{
	CCrossUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CCrossUi(nPriority);
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
HRESULT CCrossUi::Init(void)
{
	CObject2D::Init();

	//�e�N�X�`���ݒ�
	SetTexture(TEXTURE_PASS);

	SetSize(D3DXVECTOR3(SAMPLE_WIGHT, SAMPLE_HEIGHT, 0.0f));

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CCrossUi::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CCrossUi::Update(void)
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
void CCrossUi::Draw(void)
{
	CObject2D::Draw();
}

//====================================================================
//UI�̃f�t�H���g�̃T�C�Y�擾
//====================================================================
D3DXVECTOR2 CCrossUi::GetSizeDefault(void)
{
	return D3DXVECTOR2(SAMPLE_WIGHT,SAMPLE_HEIGHT);
}