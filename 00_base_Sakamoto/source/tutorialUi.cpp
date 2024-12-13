//============================================
//
//	�`���[�g���A��UI [tutorialUi.h]
//	Author:morikawa shunya
//
//============================================

#include "tutorialUi.h"

//============================================
// �萔��`
//============================================
namespace
{
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// �`�F�b�N�}�[�J�[�e�N�X�`��
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// �}�[�J�[�T�C�Y
}

//============================================
// �R���X�g���N�^
//============================================
CTutorialUi::CTutorialUi()
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
CTutorialUi* CTutorialUi::Create(const D3DXVECTOR3& pos)
{
	CTutorialUi* pTutorialUi = new CTutorialUi();

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pTutorialUi->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// �ʒu�E�T�C�Y�ݒ�
	pTutorialUi->SetPos(pos);

	return pTutorialUi;
}

//============================================
// ������
//============================================
HRESULT CTutorialUi::Init()
{
	// �T�C�Y�ݒ�
	SetSize(MARKER_SIZE);

	// �e�N�X�`���ݒ�
	SetTexture(CHECK_MARKER_TEX);

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
