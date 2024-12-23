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
#define NUM_STAGE (11)
#define MAX_TITLEUI (3)
#define NUM_SCROLL (2)

// 前方宣言
class CObject2D;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CCursorUI;
class CNumber;

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

	// 2Dの情報
	struct SStageSelect
	{
		CObject2D* m_pSelectUI;		// ボタン
		CNumber* m_pSelectNumber01;	// １の位
		CNumber* m_pSelectNumber10; // １０の位
		CObject2D* m_pMapTex;		// サムネイル
	};

public:
	CSelect();
	~CSelect();

	enum SCROLL_TYPE
	{
		SCROLL_SMOOTH = 0,	// スムーズ
		SCROLL_LAGGY,		// カクカク
		SCROLL_NONE
	};

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

	static std::vector<SStageSelect> m_StageSelect;		// ステージセレクト

	CObject2D* m_pScrollSelect[NUM_SCROLLTYPE];	// スクロールセレクト
	CObject2D* m_pTitleButton;					// 決定ボタン入力UIのポリゴン
	CObject2D* m_pTexScroll[NUM_SCROLL];		// テクスチャスクロール
	CObjmeshDome* m_pMeshDome;					// メッシュドーム

	int m_nSelect;
	int m_nStep;
	int m_nSetStage;
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
	float m_fTex[2];
	int m_nTime;
	SCROLL_TYPE m_Type;
};
#endif