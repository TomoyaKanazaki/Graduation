//==============================================================
//
//�^�C�g������[title.cpp]
//Author:����������
//
//==============================================================
#include "title.h"
#include "manager.h"
#include "wall.h"
#include "input.h"
#include "camera.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "ranking.h"
#include "titleTexture.h"
#include "edit.h"
#include "bg.h"

//�}�N����`
#define TRANS_TIME		(60 * 10)			//�J�ڂ���܂ł̎���

//�ÓI�����o�ϐ��錾
CEdit *CTitle::m_pEdit = NULL;				//�G�f�B�b�g�̏��
bool CTitle::bReset = true;				//���Z�b�g�������ǂ���

//==============================================================
//�R���X�g���N�^
//==============================================================
CTitle::CTitle()
{
	for (int nCntTex = 0; nCntTex < NUM_TITLE_TEX; nCntTex++)
	{
		m_nIdxTexture[nCntTex] = -1;		//�e�N�X�`���ԍ�
	}

	m_nCntTrans = 0;				//�J�ڂ���܂ł̎���
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTitle::~CTitle()
{

}

//==============================================================
//����������
//==============================================================
HRESULT CTitle::Init(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�J�����̏���������
	pCamera->Init();

	//�ǂ̐���
	for (int nCntWallWidth = 0; nCntWallWidth < 3; nCntWallWidth++)
	{//��
		for (int nCntWallHeight = 0; nCntWallHeight < 3; nCntWallHeight++)
		{//�c

			CWall::Create(D3DXVECTOR3(-1000.0f + (nCntWallWidth * 1000.0f), 2000.0f + (nCntWallHeight * -1000.0f), 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

		}
	}

	//�G�f�B�b�g�̐���
	m_pEdit = CEdit::Create();

	//�^�C�g���̃e�N�X�`��
	CTitleTex::Create();
	
	if (CManager::GetInstance()->GetBgm() == false)
	{

		//BGM�Đ�
		pSound->Play(pSound->SOUND_LABEL_BGM000);
	}

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void CTitle::Uninit(void)
{
	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�X�V����
//==============================================================
void CTitle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CFade *pFade = CManager::GetInstance()->GetFade();		//�t�F�[�h�̏��擾

	//if (m_nCntTrans >= TRANS_TIME)
	//{//�J�ڂ��鎞�ԂɂȂ�����

	//	CManager::GetInstance()->SetBgm(true);
	//	pFade->SetFade(CScene::MODE_RANKING);

	//	CRanking *pRanking = CScene::GetRanking();		//�����L���O�̏��擾
	//	pRanking->SetSound(true);
	//}

	//m_nCntTrans++;
}

//==============================================================
//�`�揈��
//==============================================================
void CTitle::Draw(void)
{
	
}