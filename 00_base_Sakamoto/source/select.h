//============================================
//
//	選択画面 [select.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "Scene.h"

//マクロ定義
#define NUM_SCROLLTYPE (2)
#define NUM_STAGE (3)
#define MAX_TITLEUI (3)
#define NUM_SCROLL (2)

// 前方宣言
class CObject2D;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CCursorUI;

// タイトルクラス
class CSelect : public CScene
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
	CSelect();
	~CSelect();

	static CSelect* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	void StageSelect(void);				// 選択
	void StageButton(void);				// ボタン
	void ScrollSelect(void);			// 選択
	void ScrollButton(void);			// ボタン

	// シングルトン
	static CSelect* m_pSelect;

	float m_fCountFade;

	CObject2D* m_pStageSelect[NUM_STAGE];		//ステージセレクト
	CObject2D* m_pScrollSelect[NUM_SCROLLTYPE];	//スクロールセレクト
	CObject2D* m_pTitleButton;					//ボタン入力UIのポリゴン
	CObject2D* m_pTexScroll[NUM_SCROLL];		// テクスチャスクロール
	CObjmeshDome* m_pMeshDome;					// メッシュドーム

	int m_nSelect;
	int m_nStep;
	int m_nSetStage;
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
};
#endif