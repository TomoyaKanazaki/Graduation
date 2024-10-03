//============================================
//
//	オブジェクトアニメーション2D [objectAnim2d.cpp]
//	Author:丹野竜之介
//
//============================================
#include "objectAnim2d.h"
//====================================================================
//コンストラクタ
//====================================================================
CObjectAnim2D::CObjectAnim2D(int nPriority) :CObject2D(nPriority)
{
	m_nU = 0;//横方向の枚数
	m_nV = 0;//縦方向の枚数
	m_nAnim = 0;//アニメーションの最大枚数
	m_nCurrent = 0;//現在の枚数
	m_fFramerate = 0; //更新速度
	m_fCurrentFrame = 0;
	m_bLoop = 0;//ループの有無
	m_bReverse = 0; //逆再生の有無
}

//====================================================================
//デストラクタ
//====================================================================
CObjectAnim2D::~CObjectAnim2D()
{

}

//====================================================================
//生成処理
//====================================================================
CObjectAnim2D* CObjectAnim2D::Create(D3DXVECTOR3 pos, int nU, int nV, int nAnim, bool bLoop, float fRate, int nPriority)
{
	CObjectAnim2D* pObject2D = NULL;

	if (pObject2D == NULL)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObjectAnim2D(nPriority);
	}
	pObject2D->Init();
	pObject2D->SetPos(pos);
	pObject2D->m_nU = nU;
	pObject2D->m_nV = nV;
	pObject2D->m_nAnim = nAnim;
	pObject2D->m_bLoop = bLoop;
	pObject2D->m_fFramerate = fRate;
	return pObject2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObjectAnim2D::Init(void)
{
	
	CObject2D::Init();
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObjectAnim2D::Uninit(void)
{

	CObject2D::Uninit();
}



//====================================================================
//更新処理
//====================================================================
void CObjectAnim2D::Update(void)
{
	SetVerTex();
	if (m_nCurrent < m_nAnim)
	{
		m_fCurrentFrame += 1.0f;
		if (m_fCurrentFrame > (60.0f / m_fFramerate))
		{
			m_fCurrentFrame -= (60.0f / m_fFramerate);
			m_nCurrent++;
		}
	}
	else if (m_bLoop)
	{
		m_nCurrent = 0;
	}
	
	SetAnimTex(m_nCurrent, m_nU, m_nV);
}

//====================================================================
//描画処理
//====================================================================
void CObjectAnim2D::Draw(void)
{

	CObject2D::Draw();
}