//============================================
//
//	床モデルの処理 [tile.h]
//	Author:Satone Shion
//
//============================================
#ifndef _TILE_H_
#define _TILE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//床クラス
class CTile : public CObjectX
{
public:

	CTile(int nPriority = 3);
	CTile(int nPriority, CMapSystem::GRID gridCenter);
	~CTile();

	static CTile* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CListManager<CTile>* GetList(void); // リスト取得

private:

	CMapSystem::GRID m_Grid;	//グリット番号

	// 静的メンバ変数
	static CListManager<CTile>* m_pList; // オブジェクトリスト

	// メンバ変数
	CListManager<CTile>::AIterator m_iterator; // イテレーター

};
#endif