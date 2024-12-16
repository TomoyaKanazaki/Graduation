//============================================
//
//	�`���[�g���A��UI [tutorialUi.cpp]
//	Author:morikawa shunya
//
//============================================

#include "tutorialUi.h"

//============================================
// �萔��`
//============================================
namespace
{
}

//============================================
// �R���X�g���N�^
//============================================
CTutorialUi::CTutorialUi(int nPriority) : CObject2D(nPriority)
{
}

//============================================
// �f�X�g���N�^
//============================================
CTutorialUi::~CTutorialUi()
{
}

//============================================
// ����
//============================================
CTutorialUi* CTutorialUi::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, std::string pFilename)
{
	CTutorialUi* pTutorialUi = new CTutorialUi();

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pTutorialUi->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// �ʒu�E�T�C�Y�E�e�N�X�`���ݒ�
	pTutorialUi->SetPos(pos);
	pTutorialUi->SetSize(size);
	pTutorialUi->SetTexture(pFilename);

	return pTutorialUi;
}

//============================================
// ������
//============================================
HRESULT CTutorialUi::Init()
{
	//�I�u�W�F�N�g�̏���������
	if (FAILED(CObject2D::Init()))
	{//���������������s�����ꍇ
		return E_FAIL;
	}

	return S_OK;
}

//============================================
// �I��
//============================================
void CTutorialUi::Uninit()
{
	CObject2D::Uninit();
}

//============================================
// �X�V
//============================================
void CTutorialUi::Update()
{
	CObject2D::Update();
}

//============================================
// �`��
//============================================
void CTutorialUi::Draw()
{
	CObject2D::Draw();
}
