//============================================
//
//	プレイヤーのパラメータを管理するUI [ParamererUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PARAMETERUI_H_
#define _PARAMETERUI_H_

#include "main.h"
#include "object2D.h"

// 前方宣言
class CObjGauge2D;

// マクロ定義
#define STAMINA_MAX (2)		//スタミナゲージの数

//オブジェクトプレイヤークラス
class CParameterUI : public CObject2D
{
public:

	CParameterUI(int nPriority = 6);
	~CParameterUI();

	static CParameterUI* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos);
	void SetLife(float Life);
	float GetLifeMax(void) { return m_fLifeMax; }

	void SetStamina(float Stamina);
	void SetStaminaCnt(int Count) { m_nStaminaCnt = Count; }
	int GetUseStamina(void) { return m_nUseStamina; }

private:
	void LifeGaugeCreate();			//ライフゲージ生成
	void DelLifeGaugeCreate(float Damage);		//ライフゲージ生成
	void DelLifeGaugeUpdate();		//ライフゲージ更新
	void StaminaGaugeCreate();		//スタミナゲージ生成
	void StaminaGaugeUpdate();		//スタミナゲージ更新

	D3DXVECTOR3 m_pos;				//位置

	float m_fLife;					//ライフ
	float m_fLifeMax;				//ライフの最大値
	int m_fDelLifeCoolTime;			//減少ライフのクールタイム
	float m_fDelLifeCount;			//減少ライフのカウント
	float m_fDelLifeCountMax;		//減少ライフのカウントの最大値
	float m_fStamina[STAMINA_MAX];	//スタミナ
	float m_fStaminaMax;			//スタミナ最大値
	int m_nStaminaCnt;				//スタミナ回復カウント
	int m_nUseStamina;				//使用するスタミナゲージ

	CObjGauge2D* m_pLifeGauge;			//ライフゲージのポインタ
	CObjGauge2D* m_pDelLifeGauge;		//じんわりと減るライフゲージのポインタ
	CObjGauge2D* m_pStaminaGauge[STAMINA_MAX];	//スタミナゲージのポインタ
};
#endif