//============================================
//
//	�X�R�A�̏��� [score.cpp]
//	Author:hashimoto kenta
//
//============================================
#include "score.h"
#include "number.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float SPACE_SCORE = 40.0f;		//�X�R�A���m�̌���
	const D3DXVECTOR3 SCORE_POS = D3DXVECTOR3(50.0f, 40.0f, 0.0f);		//�X�R�A�̈ʒu
	const int INIT_SCORE = 0;		//�X�R�A�̈ʒu
	const D3DXVECTOR2 SCORE_SIZE = D3DXVECTOR2(50.0f,50.0f);
}

//�ÓI�����o�ϐ��錾
CNumber* CScore::m_apObject[NUM_SCORE] = {};
D3DXVECTOR3 CScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CScore::m_nScore = 0;
int CScore::m_nNumber[NUM_SCORE] = { 0 };

//====================================================================
//�R���X�g���N�^
//====================================================================
CScore::CScore(int nPriority) : CObject(nPriority)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CScore::~CScore()
{

}

//====================================================================
//��������
//====================================================================
CScore* CScore::Create()
{
	CScore* pScore = nullptr;

	if (pScore == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pScore = new CScore();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pScore->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pScore;
}

//====================================================================
//�X�R�A�̏�����
//====================================================================
HRESULT CScore::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(SCORE_POS);

	SetScore(INIT_SCORE);

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//�����̐���
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_SCORE), m_pos.y, m_pos.z));
			m_apObject[nCntObject]->SetWidth(SCORE_SIZE.x);
			m_apObject[nCntObject]->SetHeight(SCORE_SIZE.y);

			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	return S_OK;
}

//====================================================================
//�X�R�A�̏I��
//====================================================================
void CScore::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//�����|�C���^�̏I��
//====================================================================
void CScore::SetNULL(void)
{
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
	}
}

//====================================================================
//�X�R�A�̍X�V
//====================================================================
void CScore::Update(void)
{
	m_nNumber[0] = m_nScore % 1000000 / 100000;
	m_nNumber[1] = m_nScore % 100000 / 10000;
	m_nNumber[2] = m_nScore % 10000 / 1000;
	m_nNumber[3] = m_nScore % 1000 / 100;
	m_nNumber[4] = m_nScore % 100 / 10;
	m_nNumber[5] = m_nScore % 10;

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject]->SetNumber(m_nNumber[nCntObject]);
	}
}

//====================================================================
//�X�R�A�̕`��
//====================================================================
void CScore::Draw(void)
{
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject]->Draw();
	}
}