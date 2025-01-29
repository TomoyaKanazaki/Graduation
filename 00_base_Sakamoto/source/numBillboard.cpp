//============================================
//
//	�����̏����i�r���{�[�h�j [numBillboard.cpp]
//	Author:���������Y
//
//============================================
#include "numBillboard.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float NUMBER_WIGHT = 40.0f;		//����
	const float NUMBER_HEIGHT =40.0f;		//�c��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CNumBillboard::CNumBillboard(int nPriority) : CObjectBillboard(nPriority)
{
	m_nNumber = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CNumBillboard::~CNumBillboard()
{

}

//====================================================================
//��������
//====================================================================
CNumBillboard *CNumBillboard::Create(void)
{
	CNumBillboard *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pNumber = new CNumBillboard();
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
HRESULT CNumBillboard::Init(void)
{
	CObjectBillboard::Init();

	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);

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
void CNumBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CNumBillboard::Update(void)
{
	SetAnimTex(m_nNumber);
	CObjectBillboard::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CNumBillboard::Draw(void)
{
	CObjectBillboard::Draw();
}