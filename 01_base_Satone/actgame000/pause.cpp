//==============================================================
//
//�|�[�Y����[pause.h]
//Author:����������
//
//==============================================================
#include "pause.h"
#include "manager.h"
#include "texture.h"
#include "object2D.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//�}�N����`
#define WIDTH_MULTI		(400.0f * 0.5f)		//����
#define HEIGHT_MULTI	(100.0f * 0.5f)		//�c��

//�ÓI�����o�ϐ��錾
CObject2D *CPause::m_apObject2D[PAUSE_MAX] = {};
LPDIRECT3DTEXTURE9 CPause::m_pTexture[PAUSE_MAX] = {};		//�e�N�X�`��

//==============================================================
//�R���X�g���N�^
//==============================================================
CPause::CPause()
{
	//�ϐ�������
	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
	{
		m_nIdxTexture[nCntPause] = -1;		//�e�N�X�`���̔ԍ�
	}

	m_pos = D3DXVECTOR3(300.0f, 360.0f, 0.0f);		//�ʒu

	//m_pause = PAUSE_CONTINUE;		//�|�[�Y���j���[
	m_nSelect = PAUSE_CONTINUE;		//���Ԗڂ�
	m_bPad = false;						//�p�b�h�̃X�e�B�b�N�|���Ă邩
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CPause::~CPause()
{

}

//==============================================================
//��������
//==============================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;

	if (pPause == NULL)
	{//���������g�p����ĂȂ�������

		//�|�[�Y�̐���
		pPause = new CPause;

		//����������
		pPause->Init();

		//��ސݒ�
		pPause->SetType(TYPE_PAUSE);
	}

	return pPause;
}

//==============================================================
//�|�[�Y�̏���������
//==============================================================
HRESULT CPause::Init()
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTexture[0] = pTexture->Regist("data\\TEXTURE\\pause00.png");
	m_nIdxTexture[1] = pTexture->Regist("data\\TEXTURE\\pause01.png");
	m_nIdxTexture[2] = pTexture->Regist("data\\TEXTURE\\pause02.png");

	//���������u
	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
	{
		if (m_apObject2D[nCntPause] == NULL)
		{//�g�p����ĂȂ��Ƃ�

			//2D�I�u�W�F�N�g����
			m_apObject2D[nCntPause] = CObject2D::Create();

			if (m_apObject2D[nCntPause] != NULL)
			{//�����o������

				//��ސݒ�
				m_apObject2D[nCntPause]->SetType(CObject::TYPE_PAUSE);

				//�傫���ݒ�
				m_apObject2D[nCntPause]->SetSize(WIDTH_MULTI, HEIGHT_MULTI);

				//�e�N�X�`�����蓖��
				m_apObject2D[nCntPause]->BindTexture(m_nIdxTexture[nCntPause]);

				//�ʒu�ݒ�
				m_apObject2D[nCntPause]->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + (nCntPause * HEIGHT_MULTI * 2.0f), m_pos.z));
			}
		}
	}

	//��ސݒ�
	CObject::SetType(CObject::TYPE_PAUSE);

	return S_OK;
}

//==============================================================
//�|�[�Y�̏I������
//==============================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
	{
		if (m_apObject2D[nCntPause] != NULL)
		{//�g�p����Ă�Ƃ�

			//�I������
			m_apObject2D[nCntPause]->Uninit();
			m_apObject2D[nCntPause] = NULL;
		}
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�|�[�Y�̍X�V����
//==============================================================
void CPause::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//�p�b�h�̏��擾
	CGame *pGame = CGame::GetGame();		//�Q�[���̏��擾
	CFade *pFade = CManager::GetInstance()->GetFade();		//�t�F�[�h�̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputKeyboard->GetTrigger(DIK_P) == true ||
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_START, 0) == true)
	{//P�L�[�������ꂽ

		m_nSelect = PAUSE_CONTINUE;		//�|�[�Y���j���[�̏�����
	}

	if (pInputJoyPad->GetPressRX(0).y == 0.0f &&
		pInputJoyPad->GetPressLX(0).y == 0.0f)
	{
		m_bPad = false;		//�X�e�B�b�N��|���ĂȂ���Ԃɂ���
	}

	if (pInputKeyboard->GetTrigger(DIK_W) == true ||
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_UP, 0) == true ||
		(pInputJoyPad->GetPressLX(0).y > 0.0f && m_bPad == false) ||
		(pInputJoyPad->GetPressRX(0).y > 0.0f && m_bPad == false))
	{//W�L�[�������ꂽ

		m_bPad = true;

		//BGM�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_ENTER000);

		if (m_nSelect >= PAUSE_RETRY)
		{//��֐i��

			m_nSelect--;
		}
		else if (m_nSelect == PAUSE_CONTINUE)
		{//QUIT�ɖ߂�

			m_nSelect = PAUSE_QUIT;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) == true ||
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_DOWN, 0) == true ||
		(pInputJoyPad->GetPressLX(0).y < 0.0f && m_bPad == false) ||
		(pInputJoyPad->GetPressRX(0).y < 0.0f && m_bPad == false))
	{//S�L�[�������ꂽ

		m_bPad = true;

		//BGM�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_ENTER000);

		if (m_nSelect == PAUSE_QUIT)
		{//CONTINUE�ɖ߂�

			m_nSelect = PAUSE_CONTINUE;
		}
		else if (m_nSelect >= PAUSE_CONTINUE)
		{//���֐i��

			m_nSelect++;
		}
	}

	for (int nCntPause = 0; nCntPause < PAUSE_MAX; nCntPause++)
	{
		if (m_apObject2D[nCntPause] != NULL)
		{
			//�F�ݒ�
			if (m_nSelect == nCntPause)
			{//�I�������摜��������

				m_apObject2D[m_nSelect]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			}
			else if (m_nSelect != nCntPause)
			{//�I�����ĂȂ��摜��������

				m_apObject2D[nCntPause]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

			}
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
	{//����L�[�iENTER�L�[�j�������ꂽ

		//BGM�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);

		switch (m_nSelect)
		{
		case PAUSE_CONTINUE:		//�R���e�B�j���[

			//�Q�[���ĊJ
			pGame->SetEnablePause(false);
			break;

		case PAUSE_RETRY:			//���g���C

			//�|�[�Y����
			pGame->SetEnablePause(false);

			//���[�h�ݒ�(�Q�[����ʂ̍ŏ��Ɉڍs)
			pFade->SetFade(CScene::MODE_GAME);				//�t�F�[�h�A�E�g
			break;

		case PAUSE_QUIT:			//�^�C�g��

			//�|�[�Y����
			pGame->SetEnablePause(false);

			//���[�h�ݒ�(�^�C�g����ʂɈڍs)
			pFade->SetFade(CScene::MODE_TITLE);				//�t�F�[�h�A�E�g
			break;
		}
	}
}

//==============================================================
//�|�[�Y�̕`�揈��
//==============================================================
void CPause::Draw(void)
{

}