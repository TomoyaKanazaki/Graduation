//=============================================================================
//
//���SUI���� [UI_death.h]
//Author:����������
//
//=============================================================================
#include "UI_death.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "object2D.h"
#include "texture.h"
#include "game.h"

//�}�N����`
#define PRIORITY				(7)						//�D�揇��
#define SCORE_POS_X				(120.0f)				//���SUI��X�̈ʒu
#define SCORE_POS_Y				(50.0f)					//���SUI��Y�̈ʒu
#define RESULT_SCORE_POS_X		(600.0f)				//���U���g�̎��SUI��X�̈ʒu
#define RESULT_SCORE_POS_Y		(200.0f)				//���U���g�̎��SUI��Y�̈ʒu
#define SCORE_WIDTH				(40.0f * 0.5f)			//���SUI�̉���
#define SCORE_HEIGHT			(60.0f * 0.5f)			//���SUI�̏c��

#define RESULT_SCORE_WIDTH		(60.0f * 0.5f)			//���U���g�̎��SUI�̉���
#define RESULT_SCORE_HEIGHT		(80.0f * 0.5f)			//���U���g�̎��SUI�̏c��
#define RESULT_ICON_WIDTH		(100.0f * 0.5f)			//���U���g�̃X�R�A�̉���
#define RESULT_ICON_HEIGHT		(100.0f * 0.5f)			//���U���g�̃X�R�A�̏c��

#define SCORE_INTER				(40.0f)					//���SUI�̊Ԋu
#define RESULT_SCORE_INTER		(60.0f)					//���SUI�̊Ԋu
#define NUM_TEX					(10)					//�e�N�X�`���̐����̐�
#define ICON_POS				(D3DXVECTOR3(50.0f, SCORE_POS_Y, 0.0f))				//�A�C�R���̈ʒu
#define RESULT_ICON_POS			(D3DXVECTOR3(500.0f, RESULT_SCORE_POS_Y, 0.0f))		//�A�C�R���̈ʒu

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CDeathUI::m_pTexture = NULL;		//�e�N�X�`��
CNumber *CDeathUI::m_apNumber[NUM_DEATH_DIGIT] = {};
CObject2D *CDeathUI::m_pObject2D = NULL;
int CDeathUI::m_nNum = 0;			//���SUI�̒l
int CDeathUI::m_aTexU[NUM_DEATH_DIGIT] = {};

//==============================================================
//�R���X�g���N�^
//==============================================================
CDeathUI::CDeathUI()
{
	//�ϐ�������
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		m_nIdxTex[nCntTex] = -1;		//�e�N�X�`���̔ԍ�
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_nNum = 0;		//���SUI�̒l

	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CDeathUI::~CDeathUI()
{

}

//==============================================================
//��������
//==============================================================
CDeathUI *CDeathUI::Create(void)
{
	CDeathUI *pScore = NULL;

	if (pScore == NULL)
	{//���������g�p����ĂȂ�������

		//���SUI�̐���
		pScore = new CDeathUI;
	}

	if (pScore != NULL)
	{//�������m�ۂł��Ă���

		//����������
		pScore->Init();

		if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
		{//���U���g��������
			
			m_nNum = CManager::GetInstance()->GetNumDeath();
		}

		//��ސݒ�
		pScore->SetType(TYPE_SCORE);

		//���SUI�̐ݒ�
		pScore->Set(m_nNum);
	}

	return pScore;
}

//==============================================================
//���SUI�̏���������
//==============================================================
HRESULT CDeathUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTex[TEX_DEATH] = pTexture->Regist("data\\TEXTURE\\death.png");		//���S
	m_nIdxTex[TEX_NUMBER] = pTexture->Regist("data\\TEXTURE\\number001.png");	//����

	//�A�C�R���̐���
	if (m_pObject2D == NULL)
	{//�g�p����ĂȂ��Ƃ�

		m_pObject2D = CObject2D::Create();

		if (m_pObject2D != NULL)
		{//�����ł�����

			//���S�e�N�X�`�����蓖��
			m_pObject2D->BindTexture(m_nIdxTex[TEX_DEATH]);

			//��ސݒ�
			m_pObject2D->SetType(CObject::TYPE_SCORE);

			if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
			{
				//�傫���ݒ�
				m_pObject2D->SetSize(RESULT_ICON_WIDTH, RESULT_ICON_HEIGHT);

				//�ʒu�ݒ�
				m_pObject2D->SetPosition(RESULT_ICON_POS);
			}
			else
			{
				//�傫���ݒ�
				m_pObject2D->SetSize(SCORE_HEIGHT, SCORE_HEIGHT);

				//�ʒu�ݒ�
				m_pObject2D->SetPosition(ICON_POS);
			}
		}
	}

	//�����̏���������
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] == NULL)
		{//�g�p����ĂȂ��Ƃ�

			//��������
			m_apNumber[nCntScore] = CNumber::Create();

			if (m_apNumber[nCntScore] != NULL)
			{//�����o������

				//�����e�N�X�`�����蓖��
				m_apNumber[nCntScore]->BindTexture(m_nIdxTex[TEX_NUMBER]);

				if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
				{//���U���g��������

					//�傫���ݒ�
					m_apNumber[nCntScore]->SetSize(RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);

					//���SUI�̈ʒu�ݒ�
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(RESULT_SCORE_POS_X + (nCntScore * RESULT_SCORE_INTER), RESULT_SCORE_POS_Y, 0.0f), RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);
				}
				else
				{
					//�傫���ݒ�
					m_apNumber[nCntScore]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

					//���SUI�̈ʒu�ݒ�
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(SCORE_POS_X + (nCntScore * SCORE_INTER), SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//���SUI�̏I������
//==============================================================
void CDeathUI::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{//�g�p����Ă�Ƃ�

			//�I������
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;

		}
	}

	if (m_pObject2D != NULL)
	{
		m_pObject2D->Uninit();
		m_pObject2D = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//���SUI�̍X�V����
//==============================================================
void CDeathUI::Update(void)
{
	
}

//==============================================================
//���SUI�̕`�揈��
//==============================================================
void CDeathUI::Draw(void)
{
	
}

//==============================================================
//���SUI�̐ݒ菈��
//==============================================================
void CDeathUI::Set(int nNum)
{
	int nDigit;		//����
	m_nNum = nNum;	//���SUI���

	//���SUI������
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DEATH_DIGIT - nCntScore));		//����

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//���SUI�̃e�N�X�`���ݒ�
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//���SUI�̉��Z����
//==============================================================
void CDeathUI::Add(int nValue)
{
	int nDigit;		//����
	m_nNum += nValue;	//���SUI���Z

	//���SUI������
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DEATH_DIGIT - nCntScore));		//����

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//���SUI�̃e�N�X�`���ݒ�
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//�e�N�X�`�����蓖�ď���
//==============================================================
void CDeathUI::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;		//�e�N�X�`�����蓖��
}