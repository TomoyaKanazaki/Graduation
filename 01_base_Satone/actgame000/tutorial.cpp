//==============================================================
//
//�`���[�g���A������[tutorial.cpp]
//Author:����������
//
//==============================================================
#include "tutorial.h"
#include "manager.h"
#include "wall.h"
#include "input.h"
#include "camera.h"
#include "sound.h"
#include "fade.h"
#include "object2D.h"
#include "texture.h"

#include "bg.h"

//�ÓI�����o�ϐ��錾
CPlayerModel *CTutorial::m_pPlayerModel = NULL;	//�v���C���[�̏��
CEdit *CTutorial::m_pEdit = NULL;				//�G�f�B�b�g�̏��
bool CTutorial::bReset = true;					//���Z�b�g�������ǂ���
CBulletRemain *CTutorial::m_pBulletRemain = NULL;		//�c�e���̏��
int CTutorial::m_step = STEP_SYNOP_FIRST;		//���݂̃X�e�b�v
CObject2D *CTutorial::m_pObject2D[NUM_TUTORIAL_TEX] = {};		//�I�u�W�F�N�g2D�̏��

char *CTutorial::m_apFileName[STEP_MAX] =
{
	"data\\TEXTURE\\tutorial00.jpg",
	"data\\TEXTURE\\tutorial01.jpg",
	"data\\TEXTURE\\tutorial02.jpg",
	"data\\TEXTURE\\tutorial03.jpg",
	"data\\TEXTURE\\tutorial00.png",
	"data\\TEXTURE\\tutorial01.png",
	"data\\TEXTURE\\tutorial02.png",
	"data\\TEXTURE\\tutorial04.jpg",
};

//==============================================================
//�R���X�g���N�^
//==============================================================
CTutorial::CTutorial()
{
	m_step = STEP_SYNOP_FIRST;		//���݂̃X�e�b�v

	for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		m_nIdxTexture[nCntTex] = -1;		//�e�N�X�`���ԍ�
	}

	m_nNumBullet = 0;		//�e����������
	m_bMoveR = false;		//�E�ړ�
	m_bMoveL = false;		//���ړ�
	m_bPad = false;				//�p�b�h�̃X�e�B�b�N��|������
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTutorial::~CTutorial()
{

}

//==============================================================
//����������
//==============================================================
HRESULT CTutorial::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	//�J�����̏���������
	pCamera->Init();

	CBg::Create();

	//BGM�Đ�
	pSound->Play(pSound->SOUND_LABEL_BGM001);

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void CTutorial::Uninit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//BGM�̒�~
	pSound->Stop();

	//�I�u�W�F�N�g2D�̔j��
	/*for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		if (m_pObject2D[nCntTex] != NULL)
		{
			m_pObject2D[nCntTex]->Uninit();
			m_pObject2D[nCntTex] = NULL;
		}
	}*/

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�X�V����
//==============================================================
void CTutorial::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//�p�b�h�̏��擾
	CFade *pFade = CManager::GetInstance()->GetFade();			//�t�F�[�h�̏��擾

	//�X�e�b�v����
	//CTutorial::Step();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true/* || 
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true*/)
	{//ENTER�L�[��������

		//�Q�[�����
		pFade->SetFade(CScene::MODE_GAME);

		//bReset = true;
	}
	/*else
	{
		bReset = false;
	}*/

}

//==============================================================
//�`�揈��
//==============================================================
void CTutorial::Draw(void)
{
	//�I�u�W�F�N�g2D�̕`�揈��
	/*for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		m_pObject2D[nCntTex]->Draw();
	}*/
}

//==============================================================
//�X�e�b�v����
//==============================================================
//void CTutorial::Step(void)
//{
//	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
//	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//�p�b�h�̏��擾
//	CTexture *pTexture = CManager::GetInstance()->GetTexture();
//	CSound *pSound = CManager::GetInstance()->GetSound();
//
//	switch (m_step)
//	{
//	case STEP_SYNOP_FIRST:		//���炷��1
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTER�L�[����������
//
//			//BGM�Đ�
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//�e�N�X�`���̓ǂݍ���
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//�e�N�X�`�����蓖��
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_SECOND:		//���炷��2
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTER�L�[����������
//
//			m_step++;
//
//			//BGM�Đ�
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//�e�N�X�`���̓ǂݍ���
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//�e�N�X�`�����蓖��
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_THIRD:		//���炷��3
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTER�L�[����������
//
//			//BGM�Đ�
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//�e�N�X�`���̓ǂݍ���
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//�e�N�X�`�����蓖��
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_FORTH:		//���炷��4
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTER�L�[����������
//
//			//BGM�Đ�
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			//�e�N�X�`���̓ǂݍ���
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//�e�N�X�`�����蓖��
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//
//			//�ʒu�ݒ�
//			m_pObject2D[0]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 200.0f, 0.0f), 300.0f, 100.0f);
//			m_pObject2D[1]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 1000.0f, 0.0f), 300.0f, 100.0f);
//			m_pObject2D[2]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 1000.0f, 0.0f), 300.0f, 100.0f);
//
//			//�G�f�B�b�g�̐���
//			m_pEdit = CEdit::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//
//			//�v���C���[�̐���
//			m_pPlayerModel = CPlayerModel::Create(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));
//
//			//�n��̐���
//			CWall::Create(D3DXVECTOR3(300.0f, 100.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
//		}
//
//		break;
//
//	case STEP_MOVE:		//�ړ����@
//
//		if (pInputKeyboard->GetTrigger(DIK_D) == true ||
//			pInputJoyPad->GetPressLX(0).x > 0.0f)
//		{//D�L�[����������
//
//			m_bMoveR = true;		//�E�ړ�������Ԃɂ���
//			
//		}
//		else if (pInputKeyboard->GetTrigger(DIK_A) == true ||
//			pInputJoyPad->GetPressLX(0).x < 0.0f)
//		{//D�L�[����������
//
//			m_bMoveL = true;		//�E�ړ�������Ԃɂ���
//
//		}
//
//		if (m_bMoveR == true && m_bMoveL == true)
//		{//�ړ�������
//
//			m_step++;
//
//			//�e�N�X�`���̓ǂݍ���
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//�e�N�X�`�����蓖��
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//		}
//
//		break;
//
//	case STEP_JUMP:		//�W�����v���@
//
//		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_B, 0) == true)
//		{//SPACE�L�[����������
//
//			m_step++;
//
//			//�e�N�X�`���̓ǂݍ���
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//�e�N�X�`�����蓖��
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//		}
//
//		break;
//
//	case STEP_BULLET:		//������
//
//		if (pInputJoyPad->GetPressRX(0).y == 0.0f && pInputJoyPad->GetPressRX(0).x == 0.0f)
//		{
//			m_bPad = false;		//�X�e�B�b�N��|���ĂȂ���Ԃɂ���
//		}
//
//		if (pInputKeyboard->GetTrigger(DIK_J) == true ||
//			pInputKeyboard->GetTrigger(DIK_I) == true ||
//			pInputKeyboard->GetTrigger(DIK_K) == true ||
//			pInputKeyboard->GetTrigger(DIK_L) == true ||
//			pInputKeyboard->GetTrigger(DIK_U) == true ||
//			pInputKeyboard->GetTrigger(DIK_M) == true ||
//			pInputKeyboard->GetTrigger(DIK_O) == true ||
//			pInputKeyboard->GetTrigger(DIK_PERIOD) == true ||
//			(pInputJoyPad->GetPressRX(0).x < 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressRX(0).x > 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressLX(0).y > 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressLX(0).y < 0.0f && m_bPad == false))
//		{//J�L�[����������
//
//			m_bPad = true;
//			m_nNumBullet++;
//		}
//
//		if (m_nNumBullet >= 5)
//		{
//			m_step++;
//
//			//�e�N�X�`���̓ǂݍ���
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//�e�N�X�`�����蓖��
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//
//			//�ʒu�ݒ�
//			m_pObject2D[0]->SetPosition(CObject::TYPE_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
//			m_pObject2D[1]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(1180.0f, 680.0f, 0.0f), 90.0f, 45.0f);
//			m_pObject2D[2]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(1100.0f, 60.0f, 0.0f), 150.0f, 40.0f);
//
//		}
//
//		break;
//
//	case STEP_RULE:		//���[��
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTER�L�[����������
//
//			//BGM�Đ�
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//		}
//
//		break;
//	}
//}