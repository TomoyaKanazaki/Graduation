//==============================================================
//
//背景処理[bg.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define WIDTH_BG		(1280.0f * 0.5f)		//横幅
#define HEIGHT_BG		(640.0f * 0.5f)			//縦幅

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;

//==============================================================
//コンストラクタ
//==============================================================
CBg::CBg()
{
	m_nIdxTexture = -1;		//テクスチャの番号

	m_pTexture = NULL;		//テクスチャへのポインタ
	m_pVtxBuff = NULL;		//頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
}

//==============================================================
//デストラクタ
//==============================================================
CBg::~CBg()
{

}

//==============================================================
//生成処理
//==============================================================
CBg *CBg::Create(void)
{
	CBg *pCBg = NULL;

	if (pCBg == NULL)
	{//メモリが使用されてなかったら

		//背景の生成
		pCBg = new CBg;
	}

	//初期化処理
	pCBg->Init();

	return pCBg;
}

//==============================================================
//背景の初期化処理
//==============================================================
HRESULT CBg::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	int mode = CManager::GetInstance()->GetMode();

	switch (mode)
	{
	case CScene::MODE_TITLE:

		//テクスチャの読み込み
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\title.jpg");

		break;

	case CScene::MODE_TUTORIAL:

		//テクスチャの読み込み
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\tutorial.jpg");

		break;

	case CScene::MODE_RESULT:

		//テクスチャの読み込み
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\result.jpg");

		break;

	case CScene::MODE_RANKING:

		//テクスチャの読み込み
		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\ranking.jpg");

		break;
	}

	//テクスチャの割り当て
	CObject2D::BindTexture(m_nIdxTexture);

	//オブジェクト2Dの初期化処理
	CObject2D::Init();

	//種類の設定
	CObject::SetType(CObject::TYPE_NONE);

	///背景の初期設定
	//背景の大きさ設定
	CObject2D::SetSize(WIDTH_BG, HEIGHT_BG);

	//位置設定
	CObject2D::SetPosition(m_pos);

	return S_OK;
}

//==============================================================
//背景の終了処理
//==============================================================
void CBg::Uninit(void)
{
	//2Dオブジェクトの終了処理
	CObject2D::Uninit();
}

//==============================================================
//背景の更新処理
//==============================================================
void CBg::Update(void)
{
	//2Dオブジェクトの更新処理
	CObject2D::Update();
}

//==============================================================
//背景の描画処理
//==============================================================
void CBg::Draw(void)
{
	//2Dオブジェクトの更新処理
	CObject2D::Draw();
}