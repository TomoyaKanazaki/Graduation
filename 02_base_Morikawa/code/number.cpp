//========================================
//
//シューティングアクション[Number.cpp]
//Author：森川駿弥
//
//========================================
#include "number.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//========================================
//コンストラクタ
//========================================
CNumber::CNumber() : 
	m_nNumber(0)
{

}

//========================================
//デストラクタ
//========================================
CNumber::~CNumber()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CNumber::Init(void)
{
	// 親クラスの初期化処理
	return CObject2D::Init();
}

//==========================================
//  終了処理
//==========================================
void CNumber::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CNumber::Update(void)
{
	// テクスチャ座標の設定
	SetVertexAnim(10, m_nNumber);
}

//==========================================
//  描画処理
//==========================================
void CNumber::Draw(void)
{
	// 親クラスの描画処理
	CObject2D::Draw();
}

//========================================
//ナンバー生成
//========================================
CNumber *CNumber::Create(void)
{
	// ポインタ宣言
	CNumber *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//生成
		pNumber = new CNumber;

		//初期化
		pNumber->Init();

		//テクスチャのポインタ
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		pNumber->BindTexture(pTexture->Regist("data\\TEXTURE\\number.png"));
	}

	//ポインタを返す
	return pNumber;
}

//==========================================
//  テクスチャ座標の計算処理
//==========================================
void CNumber::CalcUV()
{
}
