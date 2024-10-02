//============================================
//
//	���U���g��� [result.cpp]
//	Author:sakamoto kai
//
//============================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
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
	const D3DXVECTOR3 SCORE_TEX_POS(D3DXVECTOR3(200.0f, 400.0f, 0.0f));		//�]���_�̈ʒu
	const D3DXVECTOR3 SCORE_VALUE_POS(D3DXVECTOR3(SCORE_TEX_POS.x + 105.0f, SCORE_TEX_POS.y, 0.0f));		//�]���_�̒l�̈ʒu
	const float SCORE_DISTANCE(25.0f);
}

//�ÓI�����o�ϐ��錾
bool CResult::m_Appear = false;

//====================================================================
//�R���X�g���N�^
//====================================================================
CResult::CResult()
{
	m_pBg = nullptr;
	m_pLifeRanking = nullptr;
	m_ClearText = nullptr;
	m_pScoreTex = nullptr;
	m_apLife[6] = {};
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

	//"�]���_"���Ă����e�N�X�`���|���S��
	m_pScoreTex = CObject2D::Create();
	m_pScoreTex->SetPos(SCORE_TEX_POS);
	m_pScoreTex->SetWidth(200.0f);
	m_pScoreTex->SetHeight(80.0f);
	m_pScoreTex->SetTexture("data\\TEXTURE\\HGS\\result_score.png");

	//�S�̃����L���O(�u�����L���O�v)
	CObject2D *m_pRank = CObject2D::Create();
	m_pRank->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.80f, SCREEN_HEIGHT * 0.185f, 0.0f));
	m_pRank->SetWidth(340.0f);
	m_pRank->SetHeight(180.0f);
	m_pRank->SetTexture("data\\TEXTURE\\HGS\\ranking.png");

	//�S�̃����L���O(�X�R�A)
	m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
	m_pLifeRanking->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.35f, 0.0f));
	m_pLifeRanking->SetRanking(m_LifeData);

	//�S�̃����L���O(����)
	for (int nCntObject = 0; nCntObject < 5; nCntObject++)
	{
		CNumber *pNumRank = m_pLifeRanking->GetNumRankScore( 0, nCntObject);
		D3DXVECTOR3 RankPos = pNumRank->GetPos();
		RankPos.y += pNumRank->GetHeight() * 0.5f;

		//�����̐���
		CObject2D* m_pRank = CObject2D::Create();
		m_pRank->SetPos(D3DXVECTOR3(RankPos.x - 150.0f, RankPos.y + 17.0f, RankPos.z));
		m_pRank->SetWidth(256.0f);
		m_pRank->SetHeight(102.4f);
		m_pRank->SetTexture("data\\TEXTURE\\HGS\\RANKING_TEXT.png");
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
}

//====================================================================
//�X�V����
//====================================================================
void CResult::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CFade::SetFade(CScene::MODE_GAME);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		m_pLifeRanking->SaveRanking();
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_GAME);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		m_pLifeRanking->SaveRanking();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CResult::Draw(void)
{

}