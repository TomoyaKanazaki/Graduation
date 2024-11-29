//============================================
//
//	移動処理 [move.h]
//	Author:Satone Shion
//
//============================================
#ifndef _MOVE_H_
#define _MOVE_H_

#include "main.h"

//移動クラス
class CMove
{
public:

	CMove();
	~CMove();

	static CMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CListManager<CMove>* GetList(void); // リスト取得

private:

	// 静的メンバ変数
	static CListManager<CMove>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CMove>::AIterator m_iterator; // イテレーター

};
#endif