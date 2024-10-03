//==============================================================
//
//���U���g����[result.cpp]
//Author:����������
//
//==============================================================
#include "result.h"
#include "manager.h"
#include "wall.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "UI_death.h"
#include "UI_item.h"
#include "bg.h"

//==============================================================
//�R���X�g���N�^
//==============================================================
CResult::CResult()
{

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CResult::~CResult()
{

}

//==============================================================
//����������
//==============================================================
HRESULT CResult::Init(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//�w�i
	CBg::Create();

	//���SUI
	CDeathUI::Create();

	//�A�C�e��UI
	CItemUI::Create();

	//�X�R�A
	CScore::Create();

	////BGM�Đ�
	//if (CManager::GetResult() == true)
	//{
		pSound->Play(pSound->SOUND_LABEL_BGM003);
	//}
	//else if (CManager::GetResult() == false)
	//{
	//	pSound->Play(pSound->SOUND_LABEL_BGM004);
	//}

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void CResult::Uninit(void)
{
	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�X�V����
//==============================================================
void CResult::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//�p�b�h�̏��擾
	CFade *pFade = CManager::GetInstance()->GetFade();			//�t�F�[�h�̏��擾

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true))
	{//ENTER�L�[��������

		CManager::GetInstance()->SetBgm(false);

		//�����L���O���
		pFade->SetFade(CScene::MODE_RANKING);
	}
}

//==============================================================
//�`�揈��
//==============================================================
void CResult::Draw(void)
{

}