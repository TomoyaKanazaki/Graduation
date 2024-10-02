//============================================
//
//	画面を覆う2D [screenCover.h]
//	Author : Takeru Ogasawara
//
//============================================
#include "screenCover2D.h"
#include "manager.h"

//====================================================================
// コンストラクタ
//====================================================================
CScreenCover2D::CScreenCover2D(int nPriority) : CObject2D(nPriority)
{
	m_fade = FADE_NONE;
	m_nCount = 0;
}

//====================================================================
// デストラクタ
//====================================================================
CScreenCover2D::~CScreenCover2D()
{

}

//====================================================================
// 生成
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
// 初期化
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
// 終了
//====================================================================
void CScreenCover2D::Uninit()
{
	CObject2D::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CScreenCover2D::Update()
{
	CObject2D::Update();
}

//====================================================================
// 描画
//====================================================================
void CScreenCover2D::Draw()
{
	CObject2D::Draw();
}

//====================================================================
// フェード
//====================================================================
void CScreenCover2D::Fade(float fSpeed, int nCount)
{
	D3DXCOLOR color = GetColor();

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			color.a -= fSpeed;	//ポリゴンを透明にする

			if (color.a <= 0.0f)
			{
				color.a = 0.0f;

				m_fade = FADE_NONE;	//何もしていない状態
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
