//==============================================================
//
//ポーズ処理[pause.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _PAUSE_H_		//このマクロ定義がされていなかったら
#define _PAUSE_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//前方宣言
class CObject2D;		//オブジェクト2D

//ポーズクラス
class CPause : public CObject
{
public:

	//ポーズメニュー
	enum PAUSE
	{
		PAUSE_CONTINUE = 0,		//ゲームに戻る
		PAUSE_RETRY,			//ゲームをやり直す
		PAUSE_QUIT,				//タイトル画面に戻る
		PAUSE_MAX
	};

	CPause();	//コンストラクタ
	~CPause();	//デストラクタ

	static CPause *Create(void);		//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

private:
	static CObject2D *m_apObject2D[PAUSE_MAX];		//オブジェクト2Dのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSE_MAX];	//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;		//位置

	int m_nIdxTexture[PAUSE_MAX];		//テクスチャの番号
	PAUSE m_pause;						//ポーズメニュー
	int m_nSelect;						//選択肢
	bool m_bPad;						//パッドのスティック倒してるか
};

#endif