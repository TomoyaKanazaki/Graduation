//=============================================================================
//
//アイテムUI処理 [UI_item.h]
//Author:佐藤根詩音
//
//=============================================================================
#include "UI_item.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "object2D.h"
#include "texture.h"
#include "game.h"

//マクロ定義
#define PRIORITY				(7)						//優先順位
#define SCORE_POS_X				(120.0f)				//アイテムUIのXの位置
#define SCORE_POS_Y				(130.0f)				//アイテムUIのYの位置
#define RESULT_SCORE_POS_X		(600.0f)				//リザルトのアイテムUIのXの位置
#define RESULT_SCORE_POS_Y		(350.0f)				//リザルトのアイテムUIのYの位置
#define SCORE_WIDTH				(40.0f * 0.5f)			//アイテムUIの横幅
#define SCORE_HEIGHT			(60.0f * 0.5f)			//アイテムUIの縦幅

#define RESULT_SCORE_WIDTH		(60.0f * 0.5f)			//リザルトのアイテムUIの横幅
#define RESULT_SCORE_HEIGHT		(80.0f * 0.5f)			//リザルトのアイテムUIの縦幅
#define RESULT_ICON_WIDTH		(100.0f * 0.5f)			//リザルトのスコアの横幅
#define RESULT_ICON_HEIGHT		(100.0f * 0.5f)			//リザルトのスコアの縦幅

#define SCORE_INTER				(40.0f)					//アイテムUIの間隔
#define RESULT_SCORE_INTER		(60.0f)					//死亡UIの間隔
#define NUM_TEX					(10)					//テクスチャの数字の数
#define ICON_POS				(D3DXVECTOR3(50.0f, SCORE_POS_Y, 0.0f))				//アイコンの位置
#define RESULT_ICON_POS			(D3DXVECTOR3(500.0f, RESULT_SCORE_POS_Y, 0.0f))		//アイコンの位置

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CItemUI::m_pTexture = NULL;		//テクスチャ
CNumber *CItemUI::m_apNumber[NUM_ITEM_DIGIT] = {};
CObject2D *CItemUI::m_pObject2D = NULL;
int CItemUI::m_nNum = 0;			//アイテムUIの値
int CItemUI::m_aTexU[NUM_ITEM_DIGIT] = {};

//==============================================================
//コンストラクタ
//==============================================================
CItemUI::CItemUI()
{
	//変数初期化
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		m_nIdxTex[nCntTex] = -1;		//テクスチャの番号
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_nNum = 0;		//アイテムUIの値

	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}
}

//==============================================================
//デストラクタ
//==============================================================
CItemUI::~CItemUI()
{

}

//==============================================================
//生成処理
//==============================================================
CItemUI *CItemUI::Create(void)
{
	CItemUI *pScore = NULL;

	if (pScore == NULL)
	{//メモリが使用されてなかったら

		//アイテムUIの生成
		pScore = new CItemUI;
	}

	if (pScore != NULL)
	{//メモリ確保できてたら

		//初期化処理
		pScore->Init();

		if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
		{//リザルトだったら
			
			m_nNum = CManager::GetInstance()->GetNumItem();
		}

		//種類設定
		pScore->SetType(TYPE_SCORE);

		//アイテムUIの設定
		pScore->Set(m_nNum);
	}

	return pScore;
}

//==============================================================
//アイテムUIの初期化処理
//==============================================================
HRESULT CItemUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	m_nIdxTex[TEX_ITEM] = pTexture->Regist("data\\TEXTURE\\star.png");		//アイテム
	m_nIdxTex[TEX_NUMBER] = pTexture->Regist("data\\TEXTURE\\number000.png");		//数字

	//アイコンの生成
	if (m_pObject2D == NULL)
	{//使用されてないとき

		//生成する
		m_pObject2D = CObject2D::Create();

		if (m_pObject2D != NULL)
		{//生成できたら

			//死亡テクスチャ割り当て
			m_pObject2D->BindTexture(m_nIdxTex[TEX_ITEM]);

			//種類設定
			m_pObject2D->SetType(CObject::TYPE_SCORE);

			if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
			{
				//大きさ設定
				m_pObject2D->SetSize(RESULT_ICON_WIDTH, RESULT_ICON_HEIGHT);

				//位置設定
				m_pObject2D->SetPosition(RESULT_ICON_POS);
			}
			else
			{
				//大きさ設定
				m_pObject2D->SetSize(SCORE_HEIGHT, SCORE_HEIGHT);

				//位置設定
				m_pObject2D->SetPosition(ICON_POS);
			}
		}
	}

	//数字の初期化処理
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] == NULL)
		{//使用されてないとき

			//数字生成
			m_apNumber[nCntScore] = CNumber::Create();

			if (m_apNumber[nCntScore] != NULL)
			{//生成出来たら

				//数字テクスチャ割り当て
				m_apNumber[nCntScore]->BindTexture(m_nIdxTex[TEX_NUMBER]);

				if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
				{//リザルトだったら

					//大きさ設定
					m_apNumber[nCntScore]->SetSize(RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);

					//アイテムUIの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(RESULT_SCORE_POS_X + (nCntScore * RESULT_SCORE_INTER), RESULT_SCORE_POS_Y, 0.0f), RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);
				}
				else
				{
					//大きさ設定
					m_apNumber[nCntScore]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

					//アイテムUIの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(SCORE_POS_X + (nCntScore * SCORE_INTER), SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//アイテムUIの終了処理
//==============================================================
void CItemUI::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{//使用されてるとき

			//終了処理
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;

		}
	}

	if (m_pObject2D != NULL)
	{
		m_pObject2D->Uninit();
		m_pObject2D = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//アイテムUIの更新処理
//==============================================================
void CItemUI::Update(void)
{
	
}

//==============================================================
//アイテムUIの描画処理
//==============================================================
void CItemUI::Draw(void)
{
	
}

//==============================================================
//アイテムUIの設定処理
//==============================================================
void CItemUI::Set(int nNum)
{
	int nDigit;		//桁数
	m_nNum = nNum;	//アイテムUI代入

	//アイテムUI初期化
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_ITEM_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//アイテムUIのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//アイテムUIの加算処理
//==============================================================
void CItemUI::Add(int nValue)
{
	int nDigit;		//桁数
	m_nNum += nValue;	//アイテムUI加算

	//アイテムUI初期化
	for (int nCntScore = 0; nCntScore < NUM_ITEM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_ITEM_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//アイテムUIのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CItemUI::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;		//テクスチャ割り当て
}