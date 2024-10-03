//============================================
//
//	�������� [makimono.h]
//	Author : Takeru Ogasawara
//
//============================================
#include "scrollOpen.h"
#include "object2D.h"
#include "objGauge2D.h"

#include "manager.h"
#include "sound.h"

// �萔�錾
namespace
{
	const char* SCROLL_TEX_PATH = "data\\TEXTURE\\makimono\\fire.png";	// �����e�N�X�`���p�X
	const char* PAPER_TEX_PATH = "data\\TEXTURE\\makimono\\paper.png";	// ���e�N�X�`���p�X

	const D3DXVECTOR3 SCROLL_POS = { 1100.0f, SCREEN_HEIGHTCENTER, 0.0f };	// �����ʒu
	const D3DXVECTOR2 SCROLL_SIZE = { 600.0f, 150.0f };	// �����傫��
	const float SCROLL_OPEN_SPEED = 30.0f;	// �����J������
	const float SCROLL_OPEN_END_POS = 200.0f;	// �����I���ʒu

	const float SCROLL_CLOSE_SPEED = 50.0f;	// �������鑬��
	const float SCROLL_CLOSE_END_POS = 1190.0f;	// �����I���ʒu

	const D3DXVECTOR3 PAPER_POS = { 1200.0f, 115.0f, 0.0f };	// ���̏����ʒu
	const D3DXVECTOR2 PAPER_SIZE = { 488.0f, 50.0f };	// ���̑傫��
}

//====================================================================
// �R���X�g���N�^
//====================================================================
CScrollOpen::CScrollOpen()
{
	m_pScroll = nullptr;
	m_pPaper = nullptr;
	m_bSound = false;
	m_mode = MODE_NONE;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CScrollOpen::~CScrollOpen()
{

}

//====================================================================
// ����
//====================================================================
CScrollOpen* CScrollOpen::Create()
{
	CScrollOpen* pScroll = nullptr;

	if (pScroll == nullptr)
	{
		pScroll = new CScrollOpen;

		pScroll->Init();
	}

	return pScroll;
}

//====================================================================
// ������
//====================================================================
HRESULT CScrollOpen::Init()
{
	InitPaper();
	InitScroll();

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CScrollOpen::Uninit()
{
	if (m_pScroll)
	{
		m_pScroll->Uninit();
		m_pScroll = nullptr;
	}

	if (m_pPaper)
	{
		m_pPaper->Uninit();
		m_pPaper = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
// �X�V
//====================================================================
void CScrollOpen::Update()
{
	switch (m_mode)
	{
	case CScrollOpen::MODE_NONE:
		break;

	case CScrollOpen::MODE_OPEN:	// �J��
		Open();
		break;

	case CScrollOpen::MODE_STOP:	// �~�߂�
		break;

	case CScrollOpen::MODE_CLOSE:	// ����
		Close();
		break;

	case CScrollOpen::MODE_FADE:	// �t�F�[�h
		Fade();
		break;

	case CScrollOpen::MODE_END:		// �I��
		//Uninit();
		break;
	default:
		break;
	}
}

//====================================================================
// �`��
//====================================================================
void CScrollOpen::Draw()
{

}

//====================================================================
// �����̏�����
//====================================================================
void CScrollOpen::InitScroll()
{
	m_pScroll = CObject2D::Create();

	if (m_pScroll)
	{
		m_pScroll->SetPos(SCROLL_POS);
		m_pScroll->SetHeight(SCROLL_SIZE.x);
		m_pScroll->SetWidth(SCROLL_SIZE.y);
		m_pScroll->SetTexture(SCROLL_TEX_PATH);
	}

	if (m_mode == MODE_NONE)
	{
		m_mode = MODE_OPEN;
	}
}

//====================================================================
// ���̏�����
//====================================================================
void CScrollOpen::InitPaper()
{
	m_pPaper = CObjGauge2D::Create();

	if (m_pPaper)
	{
		m_pPaper->SetPos(PAPER_POS);
		m_pPaper->SetHeight(PAPER_SIZE.x);
		m_pPaper->SetWidth(PAPER_SIZE.y);
		m_pPaper->SetTexture(PAPER_TEX_PATH);
		m_pPaper->SetTypeVertex(m_pPaper->TYPE_VERTEX::TYPE_LEFT);
	}
}

//====================================================================
// �������J��
//====================================================================
void CScrollOpen::Open()
{
	if (!m_pScroll)
	{
		return;
	}

	if (!m_bSound)
	{
		// �J����
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SCROLL_OPEN);
		m_bSound = true;
	}

	// �ʒu�擾
	D3DXVECTOR3 pos = m_pScroll->GetPos();

	// �J�����x�����Z
	pos.x -= CManager::GetInstance()->GetGameSpeed() * SCROLL_OPEN_SPEED;

	if (pos.x < SCROLL_OPEN_END_POS)
	{// �I���ʒu�܂ŗ�����~�߂�
		pos.x = SCROLL_OPEN_END_POS;
		m_mode = MODE_STOP;
	}

	// �ݒ�
	m_pScroll->SetPos(pos);

	// ���̈ړ�
	movePaper(pos);
}

//====================================================================
// ���������
//====================================================================
void CScrollOpen::Close()
{
	if (!m_pScroll)
	{
		return;
	}

	if (m_bSound)
	{
		// �J����
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SCROLL_OPEN);
		m_bSound = false;
	}

	// �ʒu�̎擾
	D3DXVECTOR3 pos = m_pScroll->GetPos();

	// // ���鑬�x�����Z
	pos.x += CManager::GetInstance()->GetGameSpeed() * SCROLL_CLOSE_SPEED;

	if (pos.x > SCROLL_CLOSE_END_POS)
	{// �I���ʒu�܂ŗ�����I���
		pos.x = SCROLL_CLOSE_SPEED;
		m_mode = MODE_END;

		return;
	}

	// �ݒ�
	m_pScroll->SetPos(pos);

	// ���̈ړ�
	movePaper(pos);
}

//====================================================================
// �������t�F�[�h
//====================================================================
void CScrollOpen::Fade()
{
	if (!m_pScroll)
	{
		return;
	}

	// �F�̎擾
	D3DXCOLOR col = m_pScroll->GetColor();

	// �F�𔖂����Ă�
	col.a -= CManager::GetInstance()->GetGameSpeed() * SCROLL_CLOSE_SPEED;

	if (col.a <= 0.0f)
	{// �I���ʒu�܂ŗ�����I���
		col.a = 0.0f;
		m_mode = MODE_END;

		return;
	}

	// �F�̐ݒ�
	m_pScroll->SetColor(col);
}

//====================================================================
// �����J��
//====================================================================
void CScrollOpen::movePaper(D3DXVECTOR3 pos)
{
	if (!m_pPaper)
	{
		return;
	}

	// �l��ϊ�
	float fWidth = 1200.0f - pos.x;

	// �����̐ݒ�
	m_pPaper->SetWidth(fWidth);

	// �e�N�X�`���̐ݒ�
	m_pPaper->SetAnim1({ 1.0f, 1.0f });
}
