//=======================================
//
//フェード[fade.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _FADE_H_	 //このマクロ定義が定義されていなかったら
#define _FADE_H_	 //2連インクルード防止のマクロを定義する
#include "main.h"
#include "object.h"
#include "object2d.h"
#include "manager.h"

//=======================================
//フェードクラス
//=======================================
class CFade
{
public:
	enum FADE
	{//フェードの状態
		FADE_NONE = 0,	//何もしてない状態
		FADE_IN,		//フェードイン
		FADE_OUT,		//フェードアウト
		FADE_MAX
	};

	CFade();	//コンストラクタ
	~CFade();	//デストラクタ

	HRESULT Init(CScene::MODE modenext);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CScene::MODE modenext);
	void SetState(FADE state);
	static CFade *Create(CScene::MODE modenext);

private:
	//メンバ変数
	bool m_bTrans;				// 遷移するかどうか
	FADE m_State;				//フェードの状態
	CScene::MODE m_modeNext;	//次の画面(モード)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CObject2D *m_pObject2D;		// オブジェクトのポインタ
};

#endif
