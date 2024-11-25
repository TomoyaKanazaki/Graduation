//============================================
//
//	���U���g��� [result.cpp]
//	Author:sakamoto kai
//
//============================================
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "ranking.h"
#include "number.h"

//�}�N����`
namespace
{
	const char* START_OK("STARTSETSTAGE");	//�X�^�[�g���b�Z�[�W�����邩�ǂ����̊m�F
	const char* END_SET_OK("ENDSETSTAGE");	//�G���h���b�Z�[�W�����邩�ǂ����̊m�F
	const float SCORE_POSX(300.0f);
	const float SCORE_MOVEX(20.1f);
	
	const D3DXVECTOR3 RANKING_TXT_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 230.0f, 0.0f));		//�uRANKING�v�̈ʒu
	const D3DXVECTOR2 RANKING_TXT_SIZE(D3DXVECTOR2(340.0f, 180.0f));						//�uRANKING�v�̑傫��

	const D3DXVECTOR3 RANKING_NUMBER_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.46f, SCREEN_HEIGHT * 0.445f, 0.0f));	// �����L���O�Ɋ܂܂�鐔���̈ʒu
	const D3DXVECTOR2 RANKING_NUMBER_SIZE(D3DXVECTOR2(340.0f, 180.0f));						// �����L���O�Ɋ܂܂�鐔���̑傫��

	const D3DXVECTOR3 RANKING_RANK_POS(D3DXVECTOR3(-500.0f, 80.0f, 0.0f));		//�u���ʁv�̈ʒu
	const D3DXVECTOR2 RANKING_RANK_SIZE(D3DXVECTOR2(256.0f, 102.4f));			//�u���ʁv�̑傫��

	const D3DXVECTOR3 SCORE_TEX_POS(D3DXVECTOR3(500.0f, 100.0f, 0.0f));									//�X�R�A�̈ʒu
	const D3DXVECTOR2 SCORE_TEX_SIZE(D3DXVECTOR2(200.0f, 80.0f));										//�X�R�A�̑傫��
	const D3DXVECTOR3 SCORE_VALUE_POS(D3DXVECTOR3(SCORE_TEX_POS.x + 105.0f, SCORE_TEX_POS.y, 0.0f));	//�X�R�A�̒l�̈ʒu
	const float SCORE_DISTANCE(25.0f);																	//�X�R�A�Ɛ����̋���
}

//�ÓI�����o�ϐ��錾
CResult* CResult::m_pResult = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CResult::CResult()
{
	m_pBg = nullptr;
	m_pLifeRanking = nullptr;
	m_pScoreTex = nullptr;
	m_Appear = false;

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		m_apLife[nCnt] = nullptr;
	}
	m_AddScoreCount = 0;
	m_LifeData = 0;

	m_AddScoreCount = 0;
	m_LifeData = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CResult::~CResult()
{

}

//====================================================================
//�C���X�^���X�擾
//====================================================================
CResult* CResult::GetInstance(void)
{
	if (m_pResult == nullptr)
	{
		m_pResult = new CResult;
	}
	return m_pResult;
}

//====================================================================
//����������
//====================================================================
HRESULT CResult::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();;
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_RESULT);

	m_LifeData = CManager::GetInstance()->GetEndScore();

	//�w�i
	m_pBg = CObject2D::Create();
	m_pBg->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pBg->SetWidth(1280.0f);
	m_pBg->SetHeight(720.0f);
	m_pBg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	//����̃X�R�A
	for (int nCntObject = 0; nCntObject < 6; nCntObject++)
	{
		//�����̐���
		m_apLife[nCntObject] = CNumber::Create();
		m_apLife[nCntObject]->SetPos(D3DXVECTOR3(SCORE_VALUE_POS.x + (nCntObject * 45.0f), SCORE_VALUE_POS.y, SCORE_VALUE_POS.z));
		m_apLife[nCntObject]->SetWidth(60.0f);
		m_apLife[nCntObject]->SetHeight(72.0f);
		m_apLife[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	m_apLife[0]->SetNumber(m_LifeData % 1000000 / 100000);
	m_apLife[1]->SetNumber(m_LifeData % 100000 / 10000);
	m_apLife[2]->SetNumber(m_LifeData % 10000 / 1000);
	m_apLife[3]->SetNumber(m_LifeData % 1000 / 100);
	m_apLife[4]->SetNumber(m_LifeData % 100 / 10);
	m_apLife[5]->SetNumber(m_LifeData % 10 / 1);

	//"SCORE"���Ă����e�N�X�`���|���S��
	m_pScoreTex = CObject2D::Create();
	m_pScoreTex->SetPos(SCORE_TEX_POS);
	m_pScoreTex->SetWidth(SCORE_TEX_SIZE.x);
	m_pScoreTex->SetHeight(SCORE_TEX_SIZE.y);
	m_pScoreTex->SetTexture("data\\TEXTURE\\result_score.png");

	//�S�̃����L���O(�u�����L���O�v)
	CObject2D *m_pRank = CObject2D::Create();
	m_pRank->SetPos(RANKING_TXT_POS);
	m_pRank->SetWidth(RANKING_TXT_SIZE.x);
	m_pRank->SetHeight(RANKING_TXT_SIZE.y);
	m_pRank->SetTexture("data\\TEXTURE\\ranking.png");

	//�S�̃����L���O(�X�R�A)
	m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
	m_pLifeRanking->SetPos(RANKING_NUMBER_POS);
	m_pLifeRanking->SetRanking(m_LifeData);

	//�S�̃����L���O(����)
	for (int nCntObject = 0; nCntObject < 5; nCntObject++)
	{
		CNumber *pNumRank = m_pLifeRanking->GetNumRankScore(0, nCntObject);
		D3DXVECTOR3 RankPos = D3DXVECTOR3(pNumRank->GetPos().x, pNumRank->GetPos().y, pNumRank->GetPos().z);
		RankPos.y += pNumRank->GetHeight() * 0.5f;

		//�����̐���
		CObject2D* m_pRank = CObject2D::Create();
		m_pRank->SetPos(D3DXVECTOR3(RankPos.x + RANKING_RANK_POS.x, RankPos.y + RANKING_RANK_POS.y, RankPos.z));
		m_pRank->SetWidth(RANKING_RANK_SIZE.x);
		m_pRank->SetHeight(RANKING_RANK_SIZE.y);
		m_pRank->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");
		m_pRank->SetAnim(D3DXVECTOR2(0.0f, nCntObject * 0.2f),
						 D3DXVECTOR2(1.0f, nCntObject * 0.2f + 0.2f));
	}

	CManager::GetInstance()->GetCamera()->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CResult::Uninit(void)
{
	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	if (m_pResult != nullptr)
	{
		m_pResult = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CResult::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		//m_pLifeRanking->SaveRanking();
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		//m_pLifeRanking->SaveRanking();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CResult::Draw(void)
{

}