//========================================
//
//オブジェクト2D[object2D.h]
//Author：森川駿弥
//
//========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#include "main.h"
#include "object.h"

//========================================
// 定数定義
//========================================
namespace
{
const int MAX_BG = 64;	// 背景の最大数
}

//========================================
//オブジェクト2Dクラス
//========================================
class CObject2D : public CObject
{
public:
	CObject2D(int nPriority = 3);	//コンストラクタ
	~CObject2D();	//デストラクタ

	//メンバ関数
	HRESULT Init(void);		//初期化
	void Uninit(void);		//終了
	void Update(void);		//更新
	void Draw(void);		//描画
	static CObject2D *Create();		//オブジェクトの生成
	void SetSize(float fWidht, float fHeight);	//大きさ
	D3DXVECTOR2 GetSize() { return m_size; }
	void SetSizeOrigin(const D3DXVECTOR2& size) { m_sizeOrigin = size; }
	D3DXVECTOR2 GetsizeOrigin() { return m_sizeOrigin; }	// 元のサイズ

	virtual void SetVertex(void);			//頂点情報

	void BindTexture(int pTexture);				//テクスチャ割り当て
	void SetVertexAnim(int Num, int Cnt);		//テクスチャアニメーション
	void SetPosVertex(void);		//頂点情報
	void SetVertexBg(void);			//背景の頂点情報
	void SetVertexEffect(float m_fRadius, D3DXCOLOR m_col);		//エフェクトの頂点座標
	void SetVertexMoveBg(void);
	void SetScoreVertex(void);		//スコアの頂点情報
	void SetScore(int nNum[8]);
	void SetVertexColor(D3DXCOLOR col);

	void SetCol(D3DXCOLOR col) { m_col = col; }	// 色設定
	D3DXCOLOR GetCol() { return m_col; }		// 色取得

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxBuff; }		// 頂点情報の取得

private:
	//メンバ変数
	int m_nIdxTexture;		//テクスチャの番号
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		//頂点情報を格納
	D3DXCOLOR m_col;			// 色
	float m_fLength;			// 対角線の長さ
	float m_fAngle;				// 対角線の角度
	D3DXVECTOR2 m_size;			// サイズ
	D3DXVECTOR2 m_sizeOrigin;	// 元のサイズ
	int m_nCounterAnim;     //アニメーションカウンター
	int m_nPatternAnim;     //アニメーションパターンNo,
	float m_aTexV;			//テクスチャ座標の開始位置
};

#endif
