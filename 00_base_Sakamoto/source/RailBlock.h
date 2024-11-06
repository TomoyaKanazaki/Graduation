//============================================
//
//	レールブロックの処理 [RailBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "CubeBlock.h"

class CRail;

//オブジェクトメッシュフィールドクラス
class CRailBlock : public CCubeBlock
{
public:
	CRailBlock(int nPriority = 3);
	~CRailBlock();

	static CRailBlock* Create(int nMapWight, int nMapHeight, bool Edit, int Max, int *nMove);

	HRESULT Init(int nMapWight, int nMapHeight, bool Edit, int Max = 0, int* nMove = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void EditRailSet(int Number);
	void EditRailUpdate(void);

	// 静的メンバ関数
	static CListManager<CRailBlock>* GetList(void); // リスト取得

private:

	void Move(D3DXVECTOR3 *Pos);
	void RailCheck();
	void RailAddWrite();
	void RailSet(int Max, int* nMove);

	int StartWightNumber;
	int StartHeightNumber;

	bool bMoveOK[4];
	CRail* m_pTop;		// 先頭のレールへのポインタ
	CRail* m_pCur;		// 最後尾のレールへのポインタ

	// 静的メンバ変数
	static CListManager<CRailBlock>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CRailBlock>::AIterator m_iterator; // イテレーター

};
#endif