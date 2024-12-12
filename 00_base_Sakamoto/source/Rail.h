//============================================
//
//	レールの処理 [Rail.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _Rail_H_
#define _Rail_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjectX;		// オブジェクトX
class CObjGauge2D;

//レールクラス
class CRail : public CObject
{
public:

	CRail(int nPriority = 3);
	~CRail();

	// レールがある向き
	enum RAIL_POS
	{
		RAIL_POS_UP = 0,
		RAIL_POS_DOWN,
		RAIL_POS_LEFT,
		RAIL_POS_RIGHT,
		RAIL_POS_MAX,
	};

	static CRail* Create();

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	// レールの場所
	void PrevSet(RAIL_POS Set);		// 前
	void NextSet(RAIL_POS Set);		// 次

	void SetWightNumber(int Number) { m_nMapWidth = Number; }
	int GetWightNumber(void) { return m_nMapWidth; }
	void SetHeightNumber(int Number) { m_nMapHeight = Number; }
	int GetHeightNumber(void) { return m_nMapHeight; }

	void SetPrevRail(CRail *Set) { m_pPrev = Set; }		// 前のレール設定
	CRail* GetPrevRail(void) { return m_pPrev; }		// 前のレール取得
	void SetNextRail(CRail* Set) { m_pNext = Set; }		// 次のレール設定
	CRail* GetNextRail(void) { return m_pNext; }		// 次のレール取得

	void SetRailOK(int nCnt, bool Set) { m_bRail[nCnt] = Set; }
	bool GetRailOK(int nCnt) { return m_bRail[nCnt]; }
	int GetNextNumber(void) { return m_bNextNumber; }		// 次のモデルの番号

	// 静的メンバ関数
	static CListManager<CRail>* GetList(void); // リスト取得

private:

	CObjectX* m_pRailModel[2];
	bool m_bRail[RAIL_POS_MAX];
	int m_bNextNumber;

	int m_nMapWidth;	// マップの横番号
	int m_nMapHeight;	// マップの縦番号
								   
	CRail* m_pPrev;		// 前のレールへのポインタ
	CRail* m_pNext;		// 次のレールへのポインタ

	// 静的メンバ変数
	static CListManager<CRail>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRail>::AIterator m_iterator; // イテレーター

};
#endif