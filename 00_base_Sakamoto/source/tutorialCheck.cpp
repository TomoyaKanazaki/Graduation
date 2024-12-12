//============================================
//
//	�`���[�g���A���`�F�b�N [tutorialCheck.cpp]
//	Author:morikawa shunya
//
//============================================

#include "tutorialCheck.h"

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
CTutorialCheck::CTutorialCheck()
{
}

//============================================
// �f�X�g���N�^
//============================================
CTutorialCheck::~CTutorialCheck()
{
}

//============================================
// ����
//============================================
CTutorialCheck* CTutorialCheck::Create(const D3DXVECTOR3& pos)
{
	CTutorialCheck* pTutorialCheck = new CTutorialCheck();

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pTutorialCheck->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// �ʒu�E�T�C�Y�ݒ�
	pTutorialCheck->SetPos(pos);

	return pTutorialCheck;
}

//============================================
// ������
//============================================
HRESULT CTutorialCheck::Init()
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
void CTutorialCheck::Uninit()
{
	CObject2D::Uninit();
}

//============================================
// �X�V
//============================================
void CTutorialCheck::Update()
{
	CObject2D::Update();
}

//============================================
// �`��
//============================================
void CTutorialCheck::Draw()
{
	CObject2D::Draw();
}
