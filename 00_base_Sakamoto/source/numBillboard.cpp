//============================================
//
//	数字の処理（ビルボード） [numBillboard.cpp]
//	Author:澗口将太郎
//
//============================================
#include "numBillboard.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float NUMBER_WIGHT = 40.0f;		//横幅
	const float NUMBER_HEIGHT =40.0f;		//縦幅
}

//====================================================================
//コンストラクタ
//====================================================================
CNumBillboard::CNumBillboard(int nPriority) : CObjectBillboard(nPriority)
{
	m_nNumber = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CNumBillboard::~CNumBillboard()
{

}

//====================================================================
//生成処理
//====================================================================
CNumBillboard *CNumBillboard::Create(void)
{
	CNumBillboard *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumBillboard();
	}

	//オブジェクトの初期化処理
	if (FAILED(pNumber->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pNumber;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CNumBillboard::Init(void)
{
	CObjectBillboard::Init();

	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\Number_02.png");

	//タイプ設定
	SetType(CObject::TYPE_NUMBER);

	//アニメーションの初期値
	SetAnim(D3DXVECTOR2(0.2f, 0.2f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CNumBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CNumBillboard::Update(void)
{
	SetAnimTex(m_nNumber);
	CObjectBillboard::Update();
}

//====================================================================
//描画処理
//====================================================================
void CNumBillboard::Draw(void)
{
	CObjectBillboard::Draw();
}