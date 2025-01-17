//============================================
//
//	十字架アイテムのUI [crossUI.h]
//	Author:澗口将太郎
//
//============================================
#ifndef _CROSSUI_H_
#define _CROSSUI_H_

#include "main.h"
#include "object2D.h"

//オブジェクトプレイヤークラス
class CCrossUi : public CObject2D
{
public:

	CCrossUi(int nPriority = 6);
	~CCrossUi();

	static CCrossUi* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	float GetStateCount(void) { return m_fStateCountParcent; }
	void SetStateCount(int StateCount) { m_fStateCountParcent = StateCount; }

	D3DXVECTOR2 GetSizeDefault(void);	//UIのデフォルトのサイズ取得

private:
	float m_fStateCountParcent;		//十字架保持状態のカウントの割合
	D3DXVECTOR2 m_SizeDefault;		//UIのデフォルトのサイズ
};
#endif