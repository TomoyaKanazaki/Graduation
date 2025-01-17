//============================================
//
//	吹き出し型UI [popUI.h]
//	Author:澗口将太郎
//
//============================================
#ifndef _POPUI_H_
#define _POPUI_H_

#include "main.h"
#include "objectBillboard.h"

//オブジェクトプレイヤークラス
class CPopUi : public CObjectBillboard
{
public:

	CPopUi(int nPriority = 6);
	~CPopUi();

	static CPopUi* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	float GetStateCount(void) { return m_fStateCountParcent; }
	void SetStateCount(int StateCount) { m_fStateCountParcent = StateCount; }


private:
	float m_fStateCountParcent;		//十字架保持状態のカウントの割合

};
#endif