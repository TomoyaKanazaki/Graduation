//============================================
//
//	2DUI [2DUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _2DUI_H_
#define _2DUI_H_

#include "main.h"
#include "object2D.h"

//オブジェクトプレイヤークラス
class C2DUI : public CObject2D
{
public:

	C2DUI(int nPriority = 6);
	~C2DUI();

	static C2DUI* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	

	// 静的メンバ関数
	static CListManager<C2DUI>* GetList(void); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<C2DUI>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<C2DUI>::AIterator m_iterator; // イテレーター

};
#endif