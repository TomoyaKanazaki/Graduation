//=============================================================================
//
//死亡UI処理 [UI_death.h]
//Author:佐藤根詩音
//
//=============================================================================
#ifndef _UI_DEATH_H_		//このマクロ定義がされていなかったら
#define _UI_DEATH_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//マクロ定義
#define NUM_DEATH_DIGIT		(3)			//死亡の桁数

class CNumber;		//数字
class CObject2D;	//オブジェクト2D

//死亡UIクラス
class CDeathUI : public CObject
{
public:
	CDeathUI();	//コンストラクタ
	~CDeathUI();	//デストラクタ

	static CDeathUI *Create(void);	//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);		//外部からテクスチャ指定

	static void Set(int nNum);					//死亡数設定
	int Get(void) { return m_nNum; }			//死亡数取得
	static void Add(int nValue);				//死亡数足して設定

private:
	//テクスチャの種類
	enum TEX
	{
		TEX_DEATH = 0,		//死亡テクスチャ
		TEX_NUMBER,			//数字テクスチャ
		TEX_MAX
	};

	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	int m_nIdxTex[TEX_MAX];		//テクスチャの番号

	D3DXVECTOR3 m_pos;			//スコアの位置
	static int m_nNum;			//スコアの値
	static int m_aTexU[NUM_DEATH_DIGIT];

	static CNumber *m_apNumber[NUM_DEATH_DIGIT];
	static CObject2D *m_pObject2D;
};

#endif