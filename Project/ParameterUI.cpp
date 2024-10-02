//============================================
//
//	プレイヤーのパラメータを管理するUI [ParamererUI.cpp]
//	Author:sakamoto kai
//
//============================================
#include "ParameterUI.h"
#include "renderer.h"
#include "manager.h"
#include "objGauge2D.h"

//マクロ定義
#define UI_WIGHT (50.0f)		//横幅
#define UI_HEIGHT (50.0f)		//縦幅

namespace
{
	const float MAX_LIFE = 100.0f;					//体力最大値
	const float STAMINA_COST = 4.0f;				//スタミナ消費量
	const float STAMINA_RETURN_VALUE = 0.3f;		//スタミナのフレーム/回復量
	const float GAUGE_WIDTH = 420.0f;				//ゲージの長さ
	const float GAUGE_HEIGHT = 15.0f;				//ゲージの太さ
}

//====================================================================
//コンストラクタ
//====================================================================
CParameterUI::CParameterUI(int nPriority) : CObject2D(nPriority)
{
	SetWidth(UI_WIGHT);
	SetHeight(UI_HEIGHT);
	m_pos = INITVECTOR3;

	m_pLifeGauge = nullptr;
	m_pDelLifeGauge = nullptr;

	m_fLife = MAX_LIFE;
	m_fLifeMax = MAX_LIFE;

	m_fStaminaMax = STAMINA_COST;
	m_nStaminaCnt = 0;

	for (int nCnt = 0; nCnt < STAMINA_MAX; nCnt++)
	{
		m_fStamina[nCnt] = STAMINA_COST;
		m_pStaminaGauge[nCnt] = nullptr;
	}

	m_fDelLifeCount = 0.0f;
	m_fDelLifeCountMax = 0.0f;
	m_fDelLifeCoolTime = 0;
	m_nUseStamina = STAMINA_MAX;
}

//====================================================================
//デストラクタ
//====================================================================
CParameterUI::~CParameterUI()
{

}

//====================================================================
//生成処理
//====================================================================
CParameterUI* CParameterUI::Create(int nPriority)
{
	CParameterUI* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//オブジェクト2Dの生成
		pSample2D = new CParameterUI(nPriority);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample2D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CParameterUI::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\Number01.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_2DUI);

	LifeGaugeCreate();
	StaminaGaugeCreate();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CParameterUI::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CParameterUI::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();

	//ライフの更新
	DelLifeGaugeUpdate();

	//スタミナの更新
	StaminaGaugeUpdate();
}

//====================================================================
//描画処理
//====================================================================
void CParameterUI::Draw(void)
{
	CObject2D::Draw();
}

//====================================================================
//位置設定
//====================================================================
void CParameterUI::SetPos(D3DXVECTOR3 pos)
{
	CObject2D::SetPos(pos);
	m_pos = pos;

	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->SetPos(D3DXVECTOR3(m_pos.x - 270.0f, m_pos.y - 20.0f, 0.0f));
	}

	for (int nCnt = 0; nCnt < STAMINA_MAX; nCnt++)
	{
		if (m_pStaminaGauge[nCnt] != nullptr)
		{
			m_pStaminaGauge[nCnt]->SetPos(D3DXVECTOR3(m_pos.x + (-270.0f + nCnt * (m_pStaminaGauge[nCnt]->GetWidth() + 5.0f)), m_pos.y + 9.0f, 0.0f));
		}
	}
}

//====================================================================
//ライフゲージ生成
//====================================================================
void CParameterUI::LifeGaugeCreate(void)
{
	if (m_pLifeGauge == nullptr)
	{
		m_pLifeGauge = CObjGauge2D::Create();
		m_pLifeGauge->SetWidth(GAUGE_WIDTH + 5.0f);
		m_pLifeGauge->SetHeight(GAUGE_HEIGHT);
		m_pLifeGauge->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLifeMax);
	}
}

//====================================================================
//ライフの設定
//====================================================================
void CParameterUI::SetLife(float Life)
{
	float fDamage = m_fLife - Life;
	m_fLife = Life;

	DelLifeGaugeCreate(fDamage);

	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
	}
}

//====================================================================
//ライフゲージ生成
//====================================================================
void CParameterUI::DelLifeGaugeCreate(float Damage)
{
	if (m_pDelLifeGauge == nullptr && m_pLifeGauge != nullptr)
	{
		D3DXVECTOR3 LifePos = m_pLifeGauge->GetPos();
		float LifeWidth = m_pLifeGauge->GetWidth();
		float LifeRatio = m_pLifeGauge->GetRatio();

		m_pDelLifeGauge = CObjGauge2D::Create();
		m_pDelLifeGauge->SetPos(D3DXVECTOR3(LifePos.x + LifeWidth * (m_fLife / m_fLifeMax), LifePos.y, 0.0f));
		m_pDelLifeGauge->SetWidth((LifeWidth * LifeRatio) - (LifeWidth * (m_fLife / m_fLifeMax)));
		m_pDelLifeGauge->SetHeight(m_pLifeGauge->GetHeight());
		m_pDelLifeGauge->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
		m_pDelLifeGauge->SetGaugeWidth(Damage, Damage);

		m_fDelLifeCount = Damage;
		m_fDelLifeCountMax = Damage;
		m_fDelLifeCoolTime = 30;
	}
}

//====================================================================
//ライフゲージ更新
//====================================================================
void CParameterUI::DelLifeGaugeUpdate(void)
{
	if (m_pDelLifeGauge != nullptr)
	{
		// ライフ減少カウントが０の時にライフをじんわり減少させる
		if (m_fDelLifeCoolTime > 0)
		{
			m_fDelLifeCoolTime--;
		}
		else
		{
			if (m_fDelLifeCount > 0)
			{
				m_fDelLifeCount -= 1.0f;

				// 減少ゲージ設定
				m_pDelLifeGauge->SetGaugeWidth(m_fDelLifeCountMax, m_fDelLifeCount);
			}
			else
			{
				m_pDelLifeGauge->Uninit();
				m_pDelLifeGauge = nullptr;
			}
		}
	}
}

//====================================================================
//スタミナゲージ生成
//====================================================================
void CParameterUI::StaminaGaugeCreate()
{
	for (int nCnt = 0; nCnt < STAMINA_MAX; nCnt++)
	{
		if (m_pStaminaGauge[nCnt] == nullptr)
		{
			m_pStaminaGauge[nCnt] = CObjGauge2D::Create();
			m_pStaminaGauge[nCnt]->SetWidth(GAUGE_WIDTH / STAMINA_MAX);
			m_pStaminaGauge[nCnt]->SetHeight(GAUGE_HEIGHT);
			m_pStaminaGauge[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f));
			m_pStaminaGauge[nCnt]->SetGaugeWidth(m_fStaminaMax, m_fStaminaMax);
		}
	}
}

//====================================================================
//スタミナゲージの設定
//====================================================================
void CParameterUI::SetStamina(float Stamina)
{
	m_nUseStamina--;

	//スタミナゲージの位置管理
	if (m_pStaminaGauge[m_nUseStamina] != nullptr)
	{
		if (m_nUseStamina != STAMINA_MAX - 1)
		{
			m_fStamina[m_nUseStamina] = m_fStamina[m_nUseStamina + 1] - Stamina;
			m_fStamina[m_nUseStamina + 1] = Stamina;

			m_pStaminaGauge[m_nUseStamina + 1]->SetGaugeWidth(m_fStaminaMax, m_fStamina[m_nUseStamina + 1]);
			m_pStaminaGauge[m_nUseStamina]->SetGaugeWidth(m_fStaminaMax, m_fStamina[m_nUseStamina]);
		}
		else
		{
			m_fStamina[m_nUseStamina] = Stamina;
			m_pStaminaGauge[m_nUseStamina]->SetGaugeWidth(m_fStaminaMax, m_fStamina[m_nUseStamina]);
		}

		m_pStaminaGauge[m_nUseStamina]->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

//====================================================================
//スタミナゲージ生成
//====================================================================
void CParameterUI::StaminaGaugeUpdate()
{
	if (m_pStaminaGauge[m_nUseStamina] != nullptr &&
		m_nUseStamina < STAMINA_MAX)
	{
		if (m_nStaminaCnt > 0)
		{
			m_nStaminaCnt--;
		}
		else
		{
			m_fStamina[m_nUseStamina] += STAMINA_RETURN_VALUE;
			m_pStaminaGauge[m_nUseStamina]->SetGaugeWidth(m_fStaminaMax, m_fStamina[m_nUseStamina]);

			if (m_fStamina[m_nUseStamina] > STAMINA_COST)
			{
				m_pStaminaGauge[m_nUseStamina]->SetGaugeWidth(m_fStaminaMax, m_fStaminaMax);
				m_pStaminaGauge[m_nUseStamina]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f));
				m_fStamina[m_nUseStamina] = 0.0f;
				m_nUseStamina++;
				return;
			}
		}
	}
}