//=============================================================================
//
//死亡UI処理 [UI_death.h]
//Author:佐藤根詩音
//
//=============================================================================
#include "UI_death.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "object2D.h"
#include "texture.h"
#include "game.h"

//マクロ定義
#define PRIORITY				(7)						//優先順位
#define SCORE_POS_X				(120.0f)				//死亡UIのXの位置
#define SCORE_POS_Y				(50.0f)					//死亡UIのYの位置
#define RESULT_SCORE_POS_X		(600.0f)				//リザルトの死亡UIのXの位置
#define RESULT_SCORE_POS_Y		(200.0f)				//リザルトの死亡UIのYの位置
#define SCORE_WIDTH				(40.0f * 0.5f)			//死亡UIの横幅
#define SCORE_HEIGHT			(60.0f * 0.5f)			//死亡UIの縦幅

#define RESULT_SCORE_WIDTH		(60.0f * 0.5f)			//リザルトの死亡UIの横幅
#define RESULT_SCORE_HEIGHT		(80.0f * 0.5f)			//リザルトの死亡UIの縦幅
#define RESULT_ICON_WIDTH		(100.0f * 0.5f)			//リザルトのスコアの横幅
#define RESULT_ICON_HEIGHT		(100.0f * 0.5f)			//リザルトのスコアの縦幅

#define SCORE_INTER				(40.0f)					//死亡UIの間隔
#define RESULT_SCORE_INTER		(60.0f)					//死亡UIの間隔
#define NUM_TEX					(10)					//テクスチャの数字の数
#define ICON_POS				(D3DXVECTOR3(50.0f, SCORE_POS_Y, 0.0f))				//アイコンの位置
#define RESULT_ICON_POS			(D3DXVECTOR3(500.0f, RESULT_SCORE_POS_Y, 0.0f))		//アイコンの位置

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CDeathUI::m_pTexture = NULL;		//テクスチャ
CNumber *CDeathUI::m_apNumber[NUM_DEATH_DIGIT] = {};
CObject2D *CDeathUI::m_pObject2D = NULL;
int CDeathUI::m_nNum = 0;			//死亡UIの値
int CDeathUI::m_aTexU[NUM_DEATH_DIGIT] = {};

//==============================================================
//コンストラクタ
//==============================================================
CDeathUI::CDeathUI()
{
	//変数初期化
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		m_nIdxTex[nCntTex] = -1;		//テクスチャの番号
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_nNum = 0;		//死亡UIの値

	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}
}

//==============================================================
//デストラクタ
//==============================================================
CDeathUI::~CDeathUI()
{

}

//==============================================================
//生成処理
//==============================================================
CDeathUI *CDeathUI::Create(void)
{
	CDeathUI *pScore = NULL;

	if (pScore == NULL)
	{//メモリが使用されてなかったら

		//死亡UIの生成
		pScore = new CDeathUI;
	}

	if (pScore != NULL)
	{//メモリ確保できてたら

		//初期化処理
		pScore->Init();

		if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
		{//リザルトだったら
			
			m_nNum = CManager::GetInstance()->GetNumDeath();
		}

		//種類設定
		pScore->SetType(TYPE_SCORE);

		//死亡UIの設定
		pScore->Set(m_nNum);
	}

	return pScore;
}

//==============================================================
//死亡UIの初期化処理
//==============================================================
HRESULT CDeathUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	m_nIdxTex[TEX_DEATH] = pTexture->Regist("data\\TEXTURE\\death.png");		//死亡
	m_nIdxTex[TEX_NUMBER] = pTexture->Regist("data\\TEXTURE\\number001.png");	//数字

	//アイコンの生成
	if (m_pObject2D == NULL)
	{//使用されてないとき

		m_pObject2D = CObject2D::Create();

		if (m_pObject2D != NULL)
		{//生成できたら

			//死亡テクスチャ割り当て
			m_pObject2D->BindTexture(m_nIdxTex[TEX_DEATH]);

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
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
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

					//死亡UIの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(RESULT_SCORE_POS_X + (nCntScore * RESULT_SCORE_INTER), RESULT_SCORE_POS_Y, 0.0f), RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT);
				}
				else
				{
					//大きさ設定
					m_apNumber[nCntScore]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

					//死亡UIの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(SCORE_POS_X + (nCntScore * SCORE_INTER), SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//死亡UIの終了処理
//==============================================================
void CDeathUI::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
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
//死亡UIの更新処理
//==============================================================
void CDeathUI::Update(void)
{
	
}

//==============================================================
//死亡UIの描画処理
//==============================================================
void CDeathUI::Draw(void)
{
	
}

//==============================================================
//死亡UIの設定処理
//==============================================================
void CDeathUI::Set(int nNum)
{
	int nDigit;		//桁数
	m_nNum = nNum;	//死亡UI代入

	//死亡UI初期化
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DEATH_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//死亡UIのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//死亡UIの加算処理
//==============================================================
void CDeathUI::Add(int nValue)
{
	int nDigit;		//桁数
	m_nNum += nValue;	//死亡UI加算

	//死亡UI初期化
	for (int nCntScore = 0; nCntScore < NUM_DEATH_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DEATH_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//死亡UIのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CDeathUI::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;		//テクスチャ割り当て
}