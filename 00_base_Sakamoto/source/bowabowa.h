//============================================
//
//	ボワボワの処理 [SampleObjX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOWABOWA_H_
#define _BOWABOWA_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクトボワボワクラス
class CBowabowa : public CItem
{
public:

	CBowabowa(int nPriority = 3);
	~CBowabowa();

	// 静的メンバ関数
	static CListManager<CBowabowa>* GetList(void); // リスト取得

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

private:

	// 静的メンバ変数
	static CListManager<CBowabowa>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CBowabowa>::AIterator m_iterator; // イテレーター

};
#endif