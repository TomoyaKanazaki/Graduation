//============================================
//
//	��ʂ𕢂�2D [screenCover.h]
//	Author : Takeru Ogasawara
//
//============================================
#include "screenCover2D.h"
#include "manager.h"

//====================================================================
// �R���X�g���N�^
//====================================================================
CScreenCover2D::CScreenCover2D(int nPriority) : CObject2D(nPriority)
{
	m_fade = FADE_NONE;
	m_nCount = 0;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CScreenCover2D::~CScreenCover2D()
{

}

//====================================================================
// ����
//====================================================================
CScreenCover2D* CScreenCover2D::Create(int nPriority)
{
	CScreenCover2D* pCover2D = nullptr;

	if (!pCover2D)
	{
		pCover2D = new CScreenCover2D(nPriority);

		if (pCover2D)
		{
			pCover2D->Init();
		}
	}

	return pCover2D;
}

//====================================================================
// ������
//====================================================================
HRESULT CScreenCover2D::Init()
{
	CObject2D::Init();

	SetPos(SCREEN_CENTER);
	SetWidth(SCREEN_WIDTH);
	SetHeight(SCREEN_HEIGHT);
	//SetColorA(0.0f);
	m_fade = FADE_OUT;

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CScreenCover2D::Uninit()
{
	CObject2D::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CScreenCover2D::Update()
{
	CObject2D::Update();
}

//====================================================================
// �`��
//====================================================================
void CScreenCover2D::Draw()
{
	CObject2D::Draw();
}

//====================================================================
// �t�F�[�h
//====================================================================
void CScreenCover2D::Fade(float fSpeed, int nCount)
{
	D3DXCOLOR color = GetColor();

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			color.a -= fSpeed;	//�|���S���𓧖��ɂ���

			if (color.a <= 0.0f)
			{
				color.a = 0.0f;

				m_fade = FADE_NONE;	//�������Ă��Ȃ����
			}
		}
		else if (m_fade == FADE_OUT)
		{
			color.a += fSpeed;

			if (color.a >= 1.0f)
			{
				color.a = 1.0f;

				m_fade = FADE_IN;
			}
		}
	}

	SetColor(color);
}

void CScreenCover2D::SetConst(int nCount)
{
	if (nCount < 0)
	{
		nCount = 0;
	}

	m_nCount = nCount;
}
