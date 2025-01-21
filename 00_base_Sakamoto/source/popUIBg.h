//============================================
//
//	吹き出し型UI背景 [popUIBg.h]
//	Author:澗口将太郎
//
//============================================
#ifndef _POPUIBG_H_
#define _POPUIBG_H_

#include "main.h"
#include "objectBillboard.h"

//オブジェクトプレイヤークラス
class CPopUiBg : public CObjectBillboard
{
public:

	CPopUiBg(int nPriority = 6);
	~CPopUiBg();

	static CPopUiBg* Create(int nPriority = 6);

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