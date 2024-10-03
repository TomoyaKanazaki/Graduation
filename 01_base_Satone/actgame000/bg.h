//==============================================================
//
//背景処理[bg.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _BG_H_		//このマクロ定義がされていなかったら
#define _BG_H_		//2重インクルード防止のマクロを定義する

#include "object2D.h"

//プレイヤークラス
class CBg : public CObject2D
{
public:
	CBg();	//コンストラクタ
	~CBg();	//デストラクタ

	static CBg *Create(void);		//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ

	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;
};

#endif
