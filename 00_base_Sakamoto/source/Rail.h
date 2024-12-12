//============================================
//
//	レールの処理 [Rail.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _Rail_H_
#define _Rail_H_

#include "main.h"
#include "object.h"
#include "MapSystem.h"

//前方宣言
class CObjectX;		// オブジェクトX

//==========================================
//  定数定義
//==========================================
namespace Rail
{
	const int MAX_RAIL = 2;		// 1マスが持つレールの数
}

//レールクラス
class CRail : public CObject
{
public:

	CRail(int nPriority = 3);
	~CRail();

	// レール位置の種類
	enum POSTYPE
	{
		POSTYPE_NONE = 0,		// どこにも無し
		POSTYPE_UP,				// 上
		POSTYPE_DOWN,			// 下
		POSTYPE_LEFT,			// 左
		POSTYPE_RIGHT,			// 右
		POSTYPE_MAX,
	};

	static CRail* Create(CMapSystem::GRID grid, POSTYPE PosType0, POSTYPE PosType1);

	HRESULT Init(void);
	void Uninit(void);
	void SetNULL(void);
	void Update(void);
	void Draw(void);

	// レールの場所
	void PrevSet(POSTYPE Set);		// 前
	void NextSet(POSTYPE Set);		// 次

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

	CObjectX* m_pRailModel[Rail::MAX_RAIL];		// レールモデルの情報
	bool m_bRail[POSTYPE_MAX];					// レールの置ける位置の判定
	int m_bNextNumber;				// 次のレール番号

	int m_nMapWidth;	// マップの横番号
	int m_nMapHeight;	// マップの縦番号
								   
	CRail* m_pPrev;		// 前のレールへのポインタ
	CRail* m_pNext;		// 次のレールへのポインタ

	CMapSystem::GRID m_Grid;			// グリッド情報
	POSTYPE m_PosType[Rail::MAX_RAIL];	// レール位置の種類

	// 静的メンバ変数
	static CListManager<CRail>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRail>::AIterator m_iterator; // イテレーター

};
#endif