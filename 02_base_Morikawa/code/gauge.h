//========================================
//
//ゲージ[gauge.h]
//Author：森川駿弥
//
//========================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "object2D.h"

//========================================
//ゲージクラス
//========================================
class CGauge : public CObject
{
public:
	enum Gauge
	{//	ゲージ列挙
		TYPE_BASE = 0,	// 下地
		TYPE_GAUGE,		// ゲージ
		TYPE_FRAME,		// 枠線
		TYPE_MAX
	};

	CGauge(int nPriority = 7);		//コンストラクタ
	~CGauge();	//デストラクタ

	//メンバ関数
	static CGauge* Create(int nMaxLife);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVertex();
	void SetSize(float fWidht, float fHeight);
	void SetLife(int nLife) { m_nLife = nLife; }

private:
	//メンバ変数
	int m_nMaxLife;		// 体力の最大値
	int m_nLife;		// 現在の体力

	float m_fLength;		//対角線の長さ
	float m_fAngle;			//対角線の角度
	float m_aTexU;			//テクスチャのU値

	CObject2D* m_p2D[TYPE_MAX];		// オブジェクト2Dのポインタ
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャのポインタ
};

#endif
