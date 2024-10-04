//=======================================
//
//エフェクト処理[effect.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _EFFECT_H_     //このマクロ定義が定義されていなかったら
#define _EFFECT_H_     //2連インクルード防止のマクロを定義する

#include "main.h"
#include "billboard.h"

//========================================
// 定数定義
//========================================
namespace
{
const int MAX_EFFECT = 30;	// エフェクトの最大数
}

//===========================================
//エフェクトクラス
//===========================================
class CEffect : public CBillboard
{
public:
	// 構造体の定義
	struct Effect
	{
		int nLife;			// 寿命(表示時間)
		int nFirstLife;		// 初期寿命
		float fFirstAlpha;	// 初期不透明度
		bool balpha;			//加算合成
	};

	//=============================
	// 列挙型定義
	//=============================
	enum TYPE
	{
		TYPE_NORMAL = 0,	// 通常
		TYPE_POINT,			// 点
		TYPE_SMOKE,			// 煙
		TYPE_SMOKEBLACK,	// 黒煙
		TYPE_BLACK,			// 黒
		TYPE_JUJI,			// 十字
		TYPE_JUJI2,			// 十字
		TYPE_THUNDER,		// 雷
		TYPE_NULL,			// NULL
		TYPE_MAX
	};

	CEffect(int nPriority = 5);		//コンストラクタ
	~CEffect();		//デストラクタ

	//メンバ関数
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha, TYPE type);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 情報設定・取得
	void SetAddSizeValue(float value) { m_fAddSizeValue = value; }	// サイズ加算量設定
	float GetAddSizeValue() { return m_fAddSizeValue; }				// サイズ加算量取得

private:

	// 構造体の定義
	struct TexInfo
	{
		std::string textureFile;	// テクスチャファイル名
		int texID;					// テクスチャID
	};


	//=============================
	// メンバ関数
	//=============================
	// 処理系
	void Resize();	// リサイズ

	// 静的関数
	static void TexLoad();	// テクスチャ読み込み

	//メンバ変数
	Effect m_aEffect;	// 構造体の情報
	float m_fAddSizeValue;		// サイズ加算量


	static int m_TexIdx[TYPE::TYPE_MAX];	// テクスチャインデックス
	static bool m_bTexLoad;	// テクスチャの読み込み判定
};

#endif