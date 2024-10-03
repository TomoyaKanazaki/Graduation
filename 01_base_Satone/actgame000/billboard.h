//==============================================================
//
//ビルボード処理[billboard.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _BILLBOARD_H_		//このマクロ定義がされていなかったら
#define _BILLBOARD_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//ビルボードクラス
class CBillboard : public CObject
{
public:
	CBillboard();	//コンストラクタ
	~CBillboard();	//デストラクタ

	static CBillboard *Create(void);		//生成処理

	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理

	void BindTexture(int nIdxTexture);		//外部からテクスチャ指定
	void SetSize(float fWidth, float fHeight);			//オブジェクトのサイズ設定

	void SetType(int nPatternAnim);						//アニメーション
	//void SetType(float fTexU, float fTexV);			//多重背景

	void SetType(TYPE type) { m_type = type; CObject::SetType(m_type); }		//オブジェクトの種類の設定
	void SetDraw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	//位置設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	//移動量設定
	void SetColor(D3DXCOLOR col);						//色設定

	D3DXVECTOR3 GetPosition(void) { return m_pos; }		//位置取得
	D3DXVECTOR3 GetMove(void) { return m_move; }		//移動量取得
	D3DXCOLOR GetColor(void) { return m_col; }			//色取得

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXCOLOR m_col;		//色
	float m_fHeight;		//高さ
	float m_fWidth;			//横幅
	TYPE m_type;			//種類
};

#endif
