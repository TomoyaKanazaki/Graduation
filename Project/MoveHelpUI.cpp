//============================================
//
//	移動方向のヘルプUI [MoveHelpUI.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MoveHelpUI.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義
#define NUMBER_WIGHT (100.0f)		//横幅
#define NUMBER_HEIGHT (100.0f)		//縦幅

//====================================================================
//コンストラクタ
//====================================================================
CMoveHelpUI::CMoveHelpUI(int nPriority) : CObject3D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CMoveHelpUI::~CMoveHelpUI()
{

}

//====================================================================
//生成処理
//====================================================================
CMoveHelpUI* CMoveHelpUI::Create(void)
{
	CMoveHelpUI* pSample3D = NULL;

	if (pSample3D == NULL)
	{
		//オブジェクト2Dの生成
		pSample3D = new CMoveHelpUI();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample3D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CMoveHelpUI::Init(void)
{
	CObject3D::Init();

	VERTEX_3D* pVtx;	//頂点ポインタを所得
	float Width = GetWidth();
	float Height = GetHeight();
	float Rot = GetRot().y;

	//頂点バッファをロックし、両店情報へのポインタを所得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	float nLength = sqrtf(Width * Width + Height * Height) * 0.5f;
	float fAngle = atan2f(Width, Height);

	pVtx[0].pos.x = sinf(-fAngle + Rot) * nLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(-fAngle + Rot) * nLength;

	pVtx[1].pos.x = sinf(+fAngle + Rot) * nLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(+fAngle + Rot) * nLength;

	pVtx[2].pos.x = sinf(-D3DX_PI + fAngle + Rot) * nLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = cosf(-D3DX_PI + fAngle + Rot) * nLength;

	pVtx[3].pos.x = sinf(D3DX_PI - fAngle + Rot) * nLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = cosf(D3DX_PI - fAngle + Rot) * nLength;

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点バッファをアンロックする
	GetVtxBuff()->Unlock();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\MoveHelpUI.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_3DUI);

	SetLighting(false);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CMoveHelpUI::Uninit(void)
{
	CObject3D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CMoveHelpUI::Update(void)
{
	//頂点情報の更新
	CObject3D::Update();

	//頂点座標の更新
	SetVerTex();
}

//====================================================================
//描画処理
//====================================================================
void CMoveHelpUI::Draw(void)
{
	CObject3D::Draw();
}

//====================================================================
//頂点座標の設定
//====================================================================
void CMoveHelpUI::SetVerTex(void)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得
	float Width = GetWidth();
	float Height = GetHeight();
	float Rot = GetRot().z;

	//頂点バッファをロックし、両店情報へのポインタを所得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	float nLength = sqrtf(Width * Width + Height * Height) * 0.5f;
	float fAngle = atan2f(Width, Height);

	pVtx[0].pos.x = sinf(-fAngle + Rot) * nLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(-fAngle + Rot) * nLength;

	pVtx[1].pos.x = sinf(+fAngle + Rot) * nLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(+fAngle + Rot) * nLength;

	pVtx[2].pos.x = sinf(-D3DX_PI + fAngle + Rot) * nLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = cosf(-D3DX_PI + fAngle + Rot) * nLength;

	pVtx[3].pos.x = sinf(D3DX_PI - fAngle + Rot) * nLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = cosf(D3DX_PI - fAngle + Rot) * nLength;

	//頂点バッファをアンロックする
	GetVtxBuff()->Unlock();
}