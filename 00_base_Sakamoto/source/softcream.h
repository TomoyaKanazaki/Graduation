//============================================
//
//	ソフトクリームの処理 [softcream.h]
//	Author:morikawa shunya
//
//============================================
#pragma once
#include "item.h"

//==========================================
//  クラス定義
//==========================================
class CSoftCream : public CItem
{
public:

	CSoftCream(int nPriority = 3);
	~CSoftCream();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Move();

	void SetGrid(const GRID& pos) override;
	bool Hit(CPlayer* pPlayer) override;

	// 静的メンバ関数
	static CListManager<CSoftCream>* GetList(void); // リスト取得

private:

	// メンバ変数
	CListManager<CSoftCream>::AIterator m_iterator; // イテレーター

	// 静的メンバ変数
	static CListManager<CSoftCream>* m_pList; // オブジェクトリスト

};
