//==========================================
//
//	矢印(signalmanager.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once 
#include "main.h"

class CDevil;

//==========================================
//  クラス定義
//==========================================
class CSignalManager
{
public:
	// モーション
	enum TYPE_SIGNAL
	{
		// スクロール
		TYPE_SCROLL_UP,				// 傾き信号「上」
		TYPE_SCROLL_DOWN,			// 傾き信号「下」
		TYPE_SCROLL_LEFT,			// 傾き信号「左」
		TYPE_SCROLL_RIGHT,			// 傾き信号「右」
		TYPE_SCROLL_MAX,

		// スロープ
		TYPE_SLOPE_FRONT,			// 手前側
		TYPE_SLOPE_BACK,			// 奥側
		TYPE_SLOPE_RIGHT,			// 右側
		TYPE_SLOPE_LEFT,			// 左側
		TYPE_SLOPE_MAX,				// 最大
	};

	// メンバ変数
	CSignalManager(int nPriority = 2);
	~CSignalManager();

	static CSignalManager* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetMove(int nType);

	// 静的メンバ関数
	static CListManager<CSignalManager>* GetList(void); // リスト取得

private:
	int m_nIdxXModel;		//Xモデルの番号
	int m_nStateCount;		//状態管理用変数
	int m_nStateNum;
	float m_fRot;		
	CDevil* m_pDevil;

	D3DXVECTOR3 m_rot;		//回転量

	// 静的メンバ変数
	static CListManager<CSignalManager>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CSignalManager>::AIterator m_iterator; // イテレーター
};
