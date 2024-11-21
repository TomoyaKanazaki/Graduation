//============================================
//
//	十字架アイテムの処理 [cross.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CROSS_H_
#define _CROSS_H_

#include "main.h"
#include "item.h"

//前方宣言
class CObjGauge2D;

//オブジェクト十字架クラス
class CCross : public CItem
{
public:

	// メンバ関数
	CCross(int nPriority = 2);
	~CCross();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Hit(CPlayer* pPlayer) override;

	// 静的メンバ関数
	static CListManager<CCross>* GetList(void); // リスト取得

private:

	// メンバ関数
	void Rotation();
	void SetEffect() override;

	// 静的メンバ変数
	static CListManager<CCross>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CCross>::AIterator m_iterator; // イテレーター

};
#endif