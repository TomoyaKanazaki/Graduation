//=============================================================================
//
//�A�C�e��UI���� [UI_item.h]
//Author:����������
//
//=============================================================================
#include "UI_item.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "object2D.h"
#include "texture.h"
#include "game.h"

//�}�N����`
#define PRIORITY				(7)						//�D�揇��
#define SCORE_POS_X				(120.0f)				//�A�C�e��UI��X�̈ʒu
#define SCORE_POS_Y				(130.0f)				//�A�C�e��UI��Y�̈ʒu
#define RESULT_SCORE_POS_X		(600.0f)				//���U���g�̃A�C�e��UI��X�̈ʒu
#define RESULT_SCORE_POS_Y		(350.0f)				//���U���g�̃A�C�e��UI��Y�̈ʒu
#define SCORE_WIDTH				(40.0f * 0.5f)			//�A�C�e��UI�̉���
#define SCORE_HEIGHT			(60.0f * 0.5f)			//�A�C�e��UI�̏c��

#define RESULT_SCORE_WIDTH		(60.0f * 0.5f)			//���U���g�̃A�C�e��UI�̉���
#define RESULT_SCORE_HEIGHT		(80.0f * 0.5f)			//���U���g�̃A�C�e��UI�̏c��
#define RESULT_ICON_WIDTH		(100.0f * 0.5f)			//���U���g�̃X�R�A�̉���
#define RESULT_ICON_HEIGHT		(100.0f * 0.5f)			//���U���g�̃X�R�A�̏c��

#define SCORE_INTER				(40.0f)					//�A�C�e��UI�̊Ԋu
#define RESULT_SCORE_INTER		(60.0f)					//���SUI�̊Ԋu
#define NUM_TEX					(10)					//�e�N�X�`���̐����̐�
#define ICON_POS				(D3DXVECTOR3(50.0f, SCORE_POS_Y, 0.0f))				//�A�C�R���̈ʒu
#define RESULT_ICON_POS			(D3DXVECTOR3(500.0f, RESULT_SCORE_POS_Y, 0.0f))		//�A�C�R���̈ʒu

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CItemUI::m_pTexture = NULL;		//�e�N�X�`��
CNumber *CItemUI::m_apNumber[NUM_ITEM_DIGIT] = {};
CObject2D *CItemUI::m_pObject2D = NULL;
int CItemUI::m_nNum = 0;			//�A�C�e��UI�̒l
int CItemUI::m_aTexU[NUM_ITEM_DIGIT] = {};

//==============================================================
//�R���X�g���N�^
//==============================================================
CItemUI::CItemUI()
{
	//�ϐ�������
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		m_nIdxTex[nCntTex] = -1;		//�e�N�X�`���̔ԍ�
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_nNum = 0;		//�A�C�e��UI�̒l

	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CItemUI::~CItemUI()
{

}

//==============================================================
//��������
//==============================================================
CItemUI *CItemUI::Create(void)
{
	CItemUI *pScore = NULL;

	if (pScore == NULL)
	{//���������g�p����ĂȂ�������

		//�A�C�e��UI�̐���
		pScore = new CItemUI;
	}

	if (pScore != NULL)
	{//�������m�ۂł��Ă���

		//����������
		pScore->Init();

		if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
		{//���U���g��������
			
			m_nNum = CManager::GetInstance()->GetNumItem();
		}

		//��ސݒ�
		pScore->SetType(TYPE_SCORE);

		//�A�C�e��UI�̐ݒ�
		pScore->Set(m_nNum);
	}

	return pScore;
}

//==============================================================
//�A�C�e��UI�̏���������
//==============================================================
HRESULT CItemUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTex[TEX_ITEM] = pTexture->Regist("data\\TEXTURE\\star.png");		//�A�C�e��
	m_nIdxTex[TEX_NUMBER] = pTexture->Regist("data\\TEXTURE\\number000.png");		//����

	//�A�C�R���̐���
	if (m_pObject2D == NULL)
	{//�g�p����ĂȂ��Ƃ�

		//��������
		m_pObject2D = CObject2D::Create();

		if (m_pObject2D != NULL)
		{//�����ł�����

			//���S�e�N�X�`�����蓖��
			m_pObject2D->BindTexture(m_nIdxTex[TEX_ITEM]);

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
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
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

					//�A�C�e��UI�̈ʒu�ݒ�
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(RESULT_SCORE_POS_X + (nCntScore * RESULT_SCORE_INTER), RESULT_SCORE_POS_Y, 0.0f), RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);
				}
				else
				{
					//�傫���ݒ�
					m_apNumber[nCntScore]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

					//�A�C�e��UI�̈ʒu�ݒ�
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(SCORE_POS_X + (nCntScore * SCORE_INTER), SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//�A�C�e��UI�̏I������
//==============================================================
void CItemUI::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
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
//�A�C�e��UI�̍X�V����
//==============================================================
void CItemUI::Update(void)
{
	
}

//==============================================================
//�A�C�e��UI�̕`�揈��
//==============================================================
void CItemUI::Draw(void)
{
	
}

//==============================================================
//�A�C�e��UI�̐ݒ菈��
//==============================================================
void CItemUI::Set(int nNum)
{
	int nDigit;		//����
	m_nNum = nNum;	//�A�C�e��UI���

	//�A�C�e��UI������
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_ITEM_DIGIT - nCntScore));		//����

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//�A�C�e��UI�̃e�N�X�`���ݒ�
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//�A�C�e��UI�̉��Z����
//==============================================================
void CItemUI::Add(int nValue)
{
	int nDigit;		//����
	m_nNum += nValue;	//�A�C�e��UI���Z

	//�A�C�e��UI������
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_ITEM_DIGIT - nCntScore));		//����

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//�A�C�e��UI�̃e�N�X�`���ݒ�
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//�e�N�X�`�����蓖�ď���
//==============================================================
void CItemUI::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;		//�e�N�X�`�����蓖��
}