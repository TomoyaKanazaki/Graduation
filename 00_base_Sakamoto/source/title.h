//============================================
//
//	タイトル画面 [title.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "Scene.h"

//マクロ定義
#define MAX_SELECT (3)
#define MAX_TITLEUI (3)

// 前方宣言
class CObject2D;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CCursorUI;

// タイトルクラス
class CTitle : public CScene
{
private:
	static const int SLASH_2D_NUM = 4;

	// 2Dの情報
	struct SInfo2D
	{
		D3DXVECTOR3 pos;	// 位置情報
		D3DXVECTOR2 size;	// 大きさ情報
		D3DXCOLOR color;	// 色情報

		CObject2D* pObj2d;	// 2Dオブジェクト
	};

public:
	CTitle();
	~CTitle();

	static CTitle* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	void Select(void);				// 選択
	void Button(void);				// ボタン

	// シングルトン
	static CTitle* m_pTitle;

	float m_fCountFade;

	CObject2D* m_pLogo;						//タイトルロゴのポリゴン
	CObject2D* m_pModeSelect[MAX_SELECT];	//モードセレクト
	CObject2D* m_pTitleButton;				//ボタン入力UIのポリゴン
	CObjmeshDome* m_pMeshDome;				// メッシュドーム
	
	int m_nSelect;
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
};
#endif